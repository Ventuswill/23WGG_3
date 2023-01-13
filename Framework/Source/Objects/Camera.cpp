//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Camera.h"
#include "FWCore.h"
#include "GameCore.h"
#include "Renderer/Uniforms.h"

namespace fw {

Camera::Camera(GameCore* pGameCore, vec3 pos)
    : GameObject( pGameCore, "Camera", pos, nullptr, nullptr )
{
    FWCore* pFW = pGameCore->GetFramework();
    SetAspectRatio( (float)pFW->GetWindowClientWidth()/pFW->GetWindowClientHeight() );
}

Camera::~Camera()
{
}

void Camera::Update(float32 deltaTime)
{
}

void Camera::Enable(int viewID)
{
    Uniforms* pUniforms = m_pGameCore->GetUniforms();
    vec2 scaleWithAspectRatio = m_ProjectionScale * vec2( 1.0f/m_AspectRatio, 1 );
    
    // Setup uniforms.
    bgfx::setUniform( pUniforms->GetUniform("u_CameraPosition"), &m_Position, 1 );
    bgfx::setUniform( pUniforms->GetUniform("u_ProjectionScale"), &scaleWithAspectRatio, 1 );
}

} // namespace fw
