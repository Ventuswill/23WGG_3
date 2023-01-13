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
#include "GameCore.h"
#include "GameObject.h"
#include "Renderer/Mesh.h"
#include "Renderer/Uniforms.h"

namespace fw {

GameObject::GameObject(GameCore* pGameCore, std::string name, vec3 pos, Mesh* pMesh, Material* pMaterial)
    : m_pGameCore( pGameCore )
    , m_Name( name )
    , m_Position( pos )
    , m_pMesh( pMesh )
    , m_pMaterial( pMaterial )
{
}

GameObject::~GameObject()
{
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::Draw(Camera* pCamera)
{
    Uniforms* pUniforms = m_pGameCore->GetUniforms();
    bgfx::setUniform( pUniforms->GetUniform("u_Scale"), &m_Scale.x );
    bgfx::setUniform( pUniforms->GetUniform("u_Rotation"), &m_Rotation.x );
    bgfx::setUniform( pUniforms->GetUniform("u_Position"), &m_Position.x );

    mat4 worldMatrix;
    worldMatrix.CreateSRT( m_Scale, m_Rotation, m_Position );
    m_pMesh->Draw( 0, pUniforms, m_pMaterial, &worldMatrix );
}

} // namespace fw
