//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "ShaderCompilerGame.h"
#include <windows.h>
#include <atlstr.h>
#include <math.h>
#include <algorithm>

void GetFileData(const char* fullpath, WIN32_FIND_DATAA* data)
{
    HANDLE handle = FindFirstFileA( fullpath, data );

    if( handle != INVALID_HANDLE_VALUE )
    {
        FindClose( handle );
    }
}

bool IsFile1NewerThanFile2(const char* fullpath1, const char* fullpath2)
{
    WIN32_FIND_DATAA data1;
    memset( &data1, 0, sizeof(data1) );
    GetFileData( fullpath1, &data1 );

    WIN32_FIND_DATAA data2;
    memset( &data2, 0, sizeof(data2) );
    GetFileData( fullpath2, &data2 );

    if( data1.ftLastWriteTime.dwHighDateTime > data2.ftLastWriteTime.dwHighDateTime ||
        ( data1.ftLastWriteTime.dwHighDateTime == data2.ftLastWriteTime.dwHighDateTime &&
          data1.ftLastWriteTime.dwLowDateTime > data2.ftLastWriteTime.dwLowDateTime ) )
    {
        return true;
    }

    return false;
}

DWORD RunCommand(const char* command)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    
    // Start the child process. 
    bool result = CreateProcess( command, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
    assert( result != false );
    
    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );
    
    DWORD exitCode=0;
    GetExitCodeProcess( pi.hProcess, &exitCode );
    
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    return exitCode;
}

ShaderCompilerGame::ShaderCompilerGame(fw::FWCore& fwCore)
    : GameCore( fwCore )
{
    Init();
}

ShaderCompilerGame::~ShaderCompilerGame()
{
    //delete m_pEventManager;
    //delete m_pImGuiManager;
}

void ShaderCompilerGame::Init()
{
    //m_pImGuiManager = new fw::ImGuiManager( &m_FWCore, 1 );
    //m_pEventManager = new fw::EventManager();

    bool shadersNeedRebuild = false;

    // Build list of shader files in folder.
    std::vector<std::string> files = fw::GetFilesInFolder( "DataRaw\\Shaders\\", ".frag" );
    std::vector<std::string> vertFiles = fw::GetFilesInFolder( "DataRaw\\Shaders\\", ".vert" );
    files.insert( files.end(), vertFiles.begin(), vertFiles.end() );

    // Get file time stamp
    for( std::string& file : files )
    {
        std::string file1 = std::string("DataRaw\\Shaders\\") + file;
        std::string file2 = std::string("Data\\Shaders\\dx9\\") + file + ".bin";
        if( IsFile1NewerThanFile2( file1.c_str(), file2.c_str() ) )
        {
            shadersNeedRebuild = true;
            break;
        }
    }

    if( shadersNeedRebuild )
    {
        SetCurrentDirectory( "DataRaw" );
        m_ErrorCode = RunCommand( "BuildShaders.bat" );
        SetCurrentDirectory( ".." );
    }

    m_FWCore.SetQuitAtEndOfFrame();
}

void ShaderCompilerGame::StartFrame(float deltaTime)
{
    //m_pImGuiManager->StartFrame( deltaTime );
}

void ShaderCompilerGame::OnEvent(fw::Event* pEvent)
{
}

void ShaderCompilerGame::Update(float deltaTime)
{
}

void ShaderCompilerGame::Draw()
{
    //int viewID = 0;

    //bgfx::touch( 0 );

    //m_pImGuiManager->EndFrame();
}
