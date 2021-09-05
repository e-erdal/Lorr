#include "InputSystem.hh"

#include "Engine.hh"

namespace Lorr
{
    void InputSystem::Init()
    {
        ZoneScoped;

        PlatformWindow *pSurface = GetEngine()->GetWindow();

        pSurface->OnSetKeyState.connect<&InputSystem::OnSetKeyState>(this);
        pSurface->OnChar.connect<&InputSystem::OnChar>(this);
        pSurface->OnSetMouseState.connect<&InputSystem::OnSetMouseState>(this);
        // pSurface->OnSetMousePosition.connect<&InputSystem::OnSetMousePosition>( this );
    }

    void InputSystem::OnSetKeyState(Key key, ButtonState state, KeyMod mod)
    {
        ZoneScoped;

        if (state == ButtonState::Pressed)
        {
            OnKeyDown(key, mod);
        }
        else
        {
            OnKeyRelease(key, mod);
        }
    }

    void InputSystem::OnChar(uint32_t key, KeyMod mod)
    {
        ZoneScoped;

        OnCharInput(key, mod);
    }

    void InputSystem::OnSetMouseState(KeyMod mod, MouseButton key, ButtonState state, const glm::ivec2 &pos)
    {
        ZoneScoped;

        if (state == ButtonState::Pressed)
        {
            OnMouseDown(mod, key, pos);
        }
        else
        {
            OnMouseRelease(mod, key, pos);
        }
    }

    void InputSystem::OnSetMousePosition(const glm::ivec2 &pos)
    {
        ZoneScoped;

        OnMouseMove(pos);
    }
}  // namespace Lorr