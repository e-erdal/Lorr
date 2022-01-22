#include "InputManager.hh"

#include "Engine/App/Engine.hh"

#include "Engine/Debug/ImGuiHandler.hh"

#include "Engine/ECS/Components/CameraComponent.hh"
#include "Engine/Terrain/Terrain.hh"

namespace lr
{
    static eastl::unordered_map<Key, Direction> kCameraMoveLUT = { { Key::Key_W, Direction::FORWARD }, { Key::Key_S, Direction::BACKWARD },
                                                                   { Key::Key_A, Direction::LEFT },    { Key::Key_D, Direction::RIGHT },
                                                                   { Key::Key_Q, Direction::UP },      { Key::Key_E, Direction::DOWN } };

    void InputManager::OnKeyboardState(ButtonState state, Key key, KeyMod mods)
    {
        ZoneScoped;

        if (ImGuiHandler::KeyPress(state, key, mods)) return;

        Scene *pActiveScene = GetApp()->GetActiveScene();

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
    }

    void InputManager::OnMouseState(ButtonState state, MouseButton button, KeyMod mods)
    {
        ZoneScoped;

        m_MouseButtonState[button] = state;

        ImGuiHandler::MouseState(state, button, mods, m_MousePos);

        //! THIS IS ONLY FOR TESTING PURPOSES
        Scene *pActiveScene = GetApp()->GetActiveScene();
        Terrain &terrain = pActiveScene->GetEntity("entity://terrain").GetComponent<Terrain>();

        if (state == ButtonState::Pressed && button == MouseButton::BTN_2) terrain.PickMouse(m_MousePos.x, m_MousePos.y);
    }

    void InputManager::OnMousePosUpdate(const glm::ivec2 &pos, const glm::ivec2 &offset)
    {
        ZoneScoped;

        m_MousePos = pos;

        if (ImGuiHandler::MousePosition(pos)) return;

        Scene *pActiveScene = GetApp()->GetActiveScene();

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
                    return;
                }
            }
        }
    }

    void InputManager::OnMouseWheelUpdate(float x, float y)
    {
        ImGuiHandler::MouseWheelState(x, y);
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