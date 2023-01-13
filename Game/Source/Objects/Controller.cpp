//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "Controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::StartFrame()
{
    m_OldFlags = m_Flags;
}

void Controller::OnEvent(fw::Event* pEvent)
{
    if( pEvent->GetType() == fw::InputEvent::GetStaticEventType() )
    {
        fw::InputEvent* pInputEvent = static_cast<fw::InputEvent*>( pEvent );
        
        if( pInputEvent->GetDeviceType() == fw::InputEvent::DeviceType::Keyboard )
        {
            if( pInputEvent->GetDeviceState() == fw::InputEvent::DeviceState::Pressed )
            {
                switch( pInputEvent->GetKeyCode() )
                {
                case 'W': case VK_UP:       { m_Flags |= Mask::Up; }        break;
                case 'S': case VK_DOWN:     { m_Flags |= Mask::Down; }      break;
                case 'A': case VK_LEFT:     { m_Flags |= Mask::Left; }      break;
                case 'D': case VK_RIGHT:    { m_Flags |= Mask::Right; }     break;
                case 'Z':                   { m_Flags |= Mask::Action; }    break;
                }
            }

            if( pInputEvent->GetDeviceState() == fw::InputEvent::DeviceState::Released )
            {
                switch( pInputEvent->GetKeyCode() )
                {
                case 'W': case VK_UP:       { m_Flags &= ~Mask::Up; }        break;
                case 'S': case VK_DOWN:     { m_Flags &= ~Mask::Down; }      break;
                case 'A': case VK_LEFT:     { m_Flags &= ~Mask::Left; }      break;
                case 'D': case VK_RIGHT:    { m_Flags &= ~Mask::Right; }     break;
                case 'Z':                   { m_Flags &= ~Mask::Action; }    break;
                }
            }
        }
    }
}

bool Controller::IsHeld(Mask mask)
{
    return (m_Flags & mask) != 0;
}

bool Controller::WasPressed(Mask mask)
{
    return ((m_Flags & mask) != 0) && ((m_OldFlags & mask) == 0);
}

bool Controller::WasReleased(Mask mask)
{
    return ((m_Flags & mask) == 0) && ((m_OldFlags & mask) != 0);
}
