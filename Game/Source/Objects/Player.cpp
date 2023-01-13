//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "Game.h"
#include "GameCore.h"
#include "Objects/Player.h"
#include "Objects/Controller.h"

Player::Player(fw::GameCore* pGameCore, Controller* pController, std::string name, vec3 pos, fw::Mesh* pMesh, fw::Material* pMaterial)
    : fw::GameObject( pGameCore, name, pos, pMesh, pMaterial )
    , m_pController( pController )
{
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    float speed = 4.0f;

    vec2 dir;

    if( m_pController->IsHeld( Controller::Mask::Up ) )
    {
        dir.y += 1;
    }
    if( m_pController->IsHeld( Controller::Mask::Down ) )
    {
        dir.y += -1;
    }
    if( m_pController->IsHeld( Controller::Mask::Left ) )
    {
        dir.x += -1;
    }
    if( m_pController->IsHeld( Controller::Mask::Right ) )
    {
        dir.x += 1;
    }

    m_Position += dir * speed * deltaTime;

    if( m_pController->WasPressed( Controller::Mask::Action ) )
    {
        m_Position.x = fw::Random::Float( 0.0f, 10.0f );
        m_Position.y = fw::Random::Float( 0.0f, 10.0f );
    }

    dir.Normalize();
}
