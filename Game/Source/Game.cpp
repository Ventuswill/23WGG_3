//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "DataTypes.h"
#include "Game.h"
#include "Events/GameEvents.h"
#include "Objects/Player.h"
#include "Objects/Controller.h"
#include "Meshes/Shapes.h"
#include "Meshes/VertexFormats.h"

Game::Game(fw::FWCore& fwCore)
    : GameCore( fwCore )
    , m_yPos(0.0f)
{
    // General renderer settings.
    int viewID = 0;
    bgfx::setViewClear( viewID, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0 );
    bgfx::setViewRect( viewID, 0, 0, m_FWCore.GetWindowClientWidth(), m_FWCore.GetWindowClientHeight() );

    // Create some manager objects.
    m_pImGuiManager = new fw::ImGuiManager( &m_FWCore, 1 );
    m_pEventManager = new fw::EventManager();

    // Create uniforms.
    m_pUniforms = new fw::Uniforms();
    m_pUniforms->CreateUniform( "u_Position", bgfx::UniformType::Vec4 );
    m_pUniforms->CreateUniform( "u_Rotation", bgfx::UniformType::Vec4 );
    m_pUniforms->CreateUniform( "u_Scale", bgfx::UniformType::Vec4 );

    m_pUniforms->CreateUniform( "u_CameraPosition", bgfx::UniformType::Vec4 );
    m_pUniforms->CreateUniform( "u_ProjectionScale", bgfx::UniformType::Vec4 );

    m_pUniforms->CreateUniform( "u_DiffuseColor", bgfx::UniformType::Vec4 );
    m_pUniforms->CreateUniform( "u_TextureColor", bgfx::UniformType::Sampler );
    m_pUniforms->CreateUniform( "u_UVScale", bgfx::UniformType::Vec4 );
    m_pUniforms->CreateUniform( "u_UVOffset", bgfx::UniformType::Vec4 );

    m_pUniforms->CreateUniform( "u_Time", bgfx::UniformType::Vec4 );

    // Create vertex formats.
    InitVertexFormats();

    // Create some meshes.
    m_pMeshes["Triangle"] = CreateTriangleMesh();
    m_pMeshes["Square"] = CreateSquareMesh();
    m_pMeshes["Sprite"] = CreateSpriteMesh();

    // Load some shaders.
    m_pShaders["SolidColor"] = new fw::ShaderProgram( "Data/Shaders/", "SolidColor.vert.bin", "SolidColor.frag.bin" );
    m_pShaders["VertexColor"] = new fw::ShaderProgram( "Data/Shaders/", "VertexColor.vert.bin", "VertexColor.frag.bin" );
    m_pShaders["Texture"] = new fw::ShaderProgram( "Data/Shaders/", "Texture.vert.bin", "Texture.frag.bin" );

    // Load some textures.
    m_pTextures["MegaMan"] = new fw::Texture( "Data/Textures/MegaMan.png" );

    // Create some materials.
    m_pMaterials["Red"] = new fw::Material( m_pShaders["SolidColor"], nullptr, fw::color4f::Red(), false );
    m_pMaterials["Blue"] = new fw::Material( m_pShaders["SolidColor"], nullptr, fw::color4f::Blue(), false );
    m_pMaterials["Green"] = new fw::Material( m_pShaders["SolidColor"], nullptr, fw::color4f::Green(), false );
    m_pMaterials["VertexColor"] = new fw::Material( m_pShaders["VertexColor"], nullptr, fw::color4f::White(), false );
    m_pMaterials["MegaMan"] = new fw::Material( m_pShaders["Texture"], m_pTextures["MegaMan"], fw::color4f::White(), true );

    // Create a controller.
    m_pController = new Controller();

    // Create some GameObjects.
    m_pCamera = new fw::Camera( this, vec3(5,5,0) );

    m_pPlayer = new Player( this, m_pController, "Player", vec3(6,5,0), m_pMeshes["Sprite"], m_pMaterials["MegaMan"] );
    m_Objects.push_back( m_pPlayer );

    m_Objects.push_back( new fw::GameObject( this, "Object 1", vec3(0,0,0), m_pMeshes["Triangle"], m_pMaterials["VertexColor"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 2", vec3(10,10,0), m_pMeshes["Triangle"], m_pMaterials["Blue"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 3", vec3(5,5,0), m_pMeshes["Square"], m_pMaterials["VertexColor"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 4", vec3(1,1,0), m_pMeshes["Square"], m_pMaterials["VertexColor"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 5", vec3(1,9,0), m_pMeshes["Square"], m_pMaterials["Blue"] ) );

    // Create box2d World
    m_pWorld = new b2World(b2Vec2(0, -10));
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_pBody = m_pWorld->CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(1, 1);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;

    m_pBody->CreateFixture(&fixtureDef);
    m_pBody->SetTransform(b2Vec2(0,15), m_pBody->GetAngle());
}

Game::~Game()
{
    for( auto& meshPair : m_pMeshes )
    {
        delete meshPair.second;
    }

    for( auto& materialPair : m_pMaterials )
    {
        delete materialPair.second;
    }

    for( auto& texturePair : m_pTextures )
    {
        delete texturePair.second;
    }

    for( auto& shaderPair : m_pShaders )
    {
        delete shaderPair.second;
    }

    for( fw::GameObject* pObject : m_Objects )
    {
        delete pObject;
    }
    
    delete m_pCamera;
    delete m_pController;

    delete m_pUniforms;

    delete m_pEventManager;
    delete m_pImGuiManager;

    delete m_pWorld;
}

void Game::StartFrame(float deltaTime)
{
    m_pImGuiManager->StartFrame( deltaTime );

    // Reset the controller.
    m_pController->StartFrame();

    // Dispatch events.
    m_pEventManager->DispatchAllEvents( this );
}

void Game::OnEvent(fw::Event* pEvent)
{
    // Process events.
    m_pController->OnEvent( pEvent );

    // Remove object from GameObject list and delete it.
    if( pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType() )
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>( pEvent );
        fw::GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        auto it = std::find( m_Objects.begin(), m_Objects.end(), pObject );
        m_Objects.erase( it );

        delete pObject;
    }

    // Set the new aspect ratio in the camera.
    if( pEvent->GetType() == fw::WindowResizeEvent::GetStaticEventType() )
    {
        int width = m_FWCore.GetWindowClientWidth();
        int height = m_FWCore.GetWindowClientHeight();

        m_pCamera->SetAspectRatio( (float)width/height );
    }

    // Pass "WM_CHAR" events to imgui to handle text input.
    if( pEvent->GetType() == fw::OnCharEvent::GetStaticEventType() )
    {
        int character = static_cast<fw::OnCharEvent*>(pEvent)->GetValue();
        m_pImGuiManager->AddInputCharacter( character );
    }
}

void Game::Update(float deltaTime)
{
    for( fw::GameObject* pObject : m_Objects )
    {
        pObject->Update( deltaTime );
        pObject->SetPosition(vec3(pObject->GetPosition().x, m_pBody->GetPosition().y));
    }

    m_pCamera->Update( deltaTime );

    Editor_DisplayObjectList();

    m_pWorld->Step(deltaTime, 1, 1);

    if (m_pBody->GetPosition().y < -10)
    {
        m_pBody->SetTransform(b2Vec2(0, 15), m_pBody->GetAngle());
    }

    ImGui::Begin("Position");

    float pos = m_pBody->GetPosition().y;
    ImGui::Text("%f",pos);

    ImGui::End();
}

void Game::Draw()
{
    int viewID = 0;

    // Setup time uniforms.
    float time = (float)fw::GetSystemTimeSinceGameStart();
    bgfx::setUniform( m_pUniforms->GetUniform("u_Time"), &time );

    // Program the view and proj uniforms from the camera.
    m_pCamera->Enable( viewID );

    // Draw all objects.
    for( fw::GameObject* pObject : m_Objects )
    {
        pObject->Draw( m_pCamera );
    }

    m_pImGuiManager->EndFrame();
}

void Game::Editor_DisplayObjectList()
{
    ImGui::Begin( "Object List" );

    for( fw::GameObject* pObject : m_Objects )
    {
        ImGui::Text( "%s", pObject->GetName().c_str() );
    }

    ImGui::End(); // "Object List"
}
