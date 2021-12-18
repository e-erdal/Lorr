#include "InputManager.hh"

#include "Engine/App/Engine.hh"

#include "Engine/Debug/ImGuiHandler.hh"

#include "Engine/ECS/Components/CameraComponent.hh"

namespace lr
{
    void InputManager::OnKeyboardState(ButtonState state, Key key, KeyMod mods)
    {
        ZoneScoped;

        Engine *pEngine = GetEngine();
        BaseApp *pApp = GetApp();
        Scene *pActiveScene = pApp->GetActiveScene();

        if (pActiveScene)
        {
            auto view = pActiveScene->GetRegistry()->view<Component::Camera3DController>();
            for (auto &entity : view)
            {
                auto &component = view.get<Component::Camera3DController>(entity);

                bool stop = state == ButtonState::Released;
                switch (key)
                {
                    case Key::Key_W: component.m_Handle.Move(Direction::FORWARD, stop); break;
                    case Key::Key_S: component.m_Handle.Move(Direction::BACKWARD, stop); break;
                    case Key::Key_A: component.m_Handle.Move(Direction::LEFT, stop); break;
                    case Key::Key_D: component.m_Handle.Move(Direction::RIGHT, stop); break;
                    case Key::Key_Q: component.m_Handle.Move(Direction::UP, stop); break;
                    case Key::Key_E: component.m_Handle.Move(Direction::DOWN, stop); break;
                    default: break;
                }
            }
        }

        ImGuiHandler::KeyPress(state, key, mods);
    }

    void InputManager::OnMouseState(ButtonState state, MouseButton button, KeyMod mods)
    {
        ZoneScoped;

        m_MouseButtonState[button] = state;

        ImGuiHandler::MouseState(state, button, mods, m_MousePos);
    }

    void InputManager::OnMousePosUpdate(const glm::ivec2 &pos, const glm::ivec2 &offset)
    {
        ZoneScoped;

        m_MousePos = pos;

        Engine *pEngine = GetEngine();
        BaseApp *pApp = GetApp();
        Scene *pActiveScene = pApp->GetActiveScene();

        if (pActiveScene)
        {
            auto view = pActiveScene->GetRegistry()->view<Component::Camera3DController>();
            for (auto &entity : view)
            {
                auto &component = view.get<Component::Camera3DController>(entity);

                bool dragAvailable = false;

                auto buttonIt = m_MouseButtonState.find(MouseButton::BTN_1);
                if (buttonIt == m_MouseButtonState.end())
                    dragAvailable = false;
                else if (buttonIt->second == ButtonState::Pressed)
                    dragAvailable = true;

                if (dragAvailable)
                {
                    component.m_Handle.SetDirection(offset.x, offset.y);
                }
            }
        }
    }

    void InputManager::OnKeyInput()
    {
        ZoneScoped;

    }

    void InputManager::SetMousePos(const glm::ivec2 &pos)
    {
        m_MousePos = pos;
    }

}  // namespace lr