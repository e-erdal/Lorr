#include "InputManager.hh"

#include "Engine/App/Engine.hh"

#include "Engine/Debug/ImGuiHandler.hh"

#include "Engine/ECS/Components/CameraComponent.hh"

namespace lr
{
    static eastl::unordered_map<Key, Direction> kCameraMoveLUT = { { Key::Key_W, Direction::FORWARD }, { Key::Key_S, Direction::BACKWARD },
                                                                 { Key::Key_A, Direction::LEFT },    { Key::Key_D, Direction::RIGHT },
                                                                 { Key::Key_Q, Direction::UP },      { Key::Key_E, Direction::DOWN } };

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
                auto keyIt = kCameraMoveLUT.find(key);
                if (keyIt != kCameraMoveLUT.end())
                {
                    component.m_Handle.Move(keyIt->second, stop);
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
                    continue;
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