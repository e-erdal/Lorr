//
// Created on Monday 6th December 2021 by e-erdal
//

#pragma once

#include "InputVars.hh"

namespace lr
{
    class InputManager
    {
    public:
        InputManager() = default;

        void OnKeyboardState(ButtonState state, Key key, KeyMod mods);
        void OnMouseState(ButtonState state, MouseButton button, KeyMod mods);
        void OnMousePosUpdate(const glm::ivec2 &pos, const glm::ivec2 &offset);
        void OnMouseWheelUpdate(float x, float y);

        /// This should be used when keyboard is active
        void OnKeyInput();

    public:
        void SetMousePos(const glm::ivec2 &pos);
        const glm::ivec2 &GetMousePos() const
        {
            return m_MousePos;
        }

    private:
        glm::ivec2 m_MousePos;
        eastl::unordered_map<MouseButton, ButtonState> m_MouseButtonState;
    };

}  // namespace lr