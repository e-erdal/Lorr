#include "InputSystem.hh"

#include "Engine.hh"

namespace Lorr
{
    void InputSystem::Init()
    {
        Window *pSurface = GetEngine()->GetWindow();

        pSurface->OnSetKeyState.connect<&InputSystem::OnSetKeyState>( this );
        pSurface->OnChar.connect<&InputSystem::OnChar>( this );
        pSurface->OnSetMouseState.connect<&InputSystem::OnSetMouseState>( this );
        pSurface->OnSetMousePosition.connect<&InputSystem::OnSetMousePosition>( this );
    }

    void InputSystem::OnSetKeyState( Key eKey, ButtonState eState, KeyMod eMod )
    {
        if ( eState == ButtonState::Pressed )
        {
            OnKeyDown( eKey, eMod );
        }
        else
        {
            OnKeyRelease( eKey, eMod );
        }
    }

    void InputSystem::OnChar( uint32_t uChar, KeyMod eMod )
    {
        OnCharInput( uChar, eMod );
    }

    void InputSystem::OnSetMouseState( KeyMod eMod, MouseButton eKey, ButtonState eState, const glm::ivec2 &ivPos )
    {
        if ( eState == ButtonState::Pressed )
        {
            OnMouseDown( eMod, eKey, ivPos );
        }
        else
        {
            OnMouseRelease( eMod, eKey, ivPos );
        }
    }

    void InputSystem::OnSetMousePosition( const glm::ivec2 &ivPos )
    {
        OnMouseMove( ivPos );
    }
}  // namespace Lorr