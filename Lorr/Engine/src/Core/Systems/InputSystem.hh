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
        signals::signal<void(KeyMod, MouseButton, const glm::ivec2 &)> OnMouseDown;
        signals::signal<void(KeyMod, MouseButton, const glm::ivec2 &)> OnMouseRelease;
        signals::signal<void(const glm::ivec2 &)> OnMouseMove;
        signals::signal<void(const glm::vec2 &)> OnMouseScroll;

        // Key when it's down and sending continuous messages, aka almost no delay
        signals::signal<void(Key, KeyMod)> OnKeyDown;
        // Key when it's released and sending continuous messages, aka almost no
        // delay
        signals::signal<void(Key, KeyMod)> OnKeyRelease;

        // This is when writing made just right for text boxes
        signals::signal<void(uint32_t, KeyMod)> OnCharInput;

    private:
        glm::vec2 m_v2MouseScrollAxis;
        glm::vec2 m_v2MouseMoveDelta;

        uint8_t m_eMouseButtonState;
        KeyMod m_eKeyMods;

    private:
        void OnSetKeyState(Key, ButtonState, KeyMod);
        void OnChar(uint32_t, KeyMod);
        void OnSetMouseState(KeyMod, MouseButton, ButtonState, const glm::ivec2 &);
        void OnSetMousePosition(const glm::ivec2 &);
    };

}  // namespace Lorr