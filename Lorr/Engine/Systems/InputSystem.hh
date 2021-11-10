//
// Created on July 13th 2021 by e-erdal.
//

#pragma once

#include "Helpers/InputVars.hh"

namespace Lorr
{
    class InputSystem
    {
    public:
        void Init();

    public:
        sig::signal<void(KeyMod, MouseButton, const glm::ivec2 &)> OnMouseDown;
        sig::signal<void(KeyMod, MouseButton, const glm::ivec2 &)> OnMouseRelease;
        sig::signal<void(const glm::ivec2 &)> OnMouseMove;
        sig::signal<void(const glm::vec2 &)> OnMouseScroll;

        // Key when it's down and sending continuous messages, aka almost no delay
        sig::signal<void(Key, KeyMod)> OnKeyDown;
        // Key when it's released and sending continuous messages, aka almost no
        // delay
        sig::signal<void(Key, KeyMod)> OnKeyRelease;

        // This is when writing made just right for text boxes
        sig::signal<void(u32, KeyMod)> OnCharInput;

    private:
        glm::vec2 m_MouseScrollAxis;
        glm::vec2 m_MouseMoveDelta;

        u8 m_MouseButtonState;
        KeyMod m_KeyMods;

    private:
        void OnSetKeyState(Key, ButtonState, KeyMod);
        void OnChar(u32, KeyMod);
        void OnSetMouseState(KeyMod, MouseButton, ButtonState, const glm::ivec2 &);
        void OnSetMousePosition(const glm::ivec2 &);
    };

}  // namespace Lorr