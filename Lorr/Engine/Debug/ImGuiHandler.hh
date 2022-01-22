//
// Created on July 10th 2021 by e-erdal.
//

#pragma once

#include "Engine/Core/Window/PlatformWindow.hh"

#include "Engine/Graphics/Common/BaseRenderBuffer.hh"
#include "Engine/Graphics/Common/BaseShader.hh"
#include "Engine/Graphics/Common/BaseTexture.hh"

#include "Engine/Managers/InputVars.hh"

#include <imgui.h>

namespace ImGui
{
    void PushFont(const lr::Identifier &ident);
}

namespace lr
{
    class Engine;
    class ImGuiHandler
    {
    public:
        ~ImGuiHandler();

        void Init(Engine *pEngine);

        void BeginFrame();
        void EndFrame();

        static bool KeyPress(ButtonState state, Key key, KeyMod mods);
        static void InputChar(u32 character, KeyMod mods);
        static bool MouseState(ButtonState state, MouseButton button, KeyMod mods, const glm::ivec2 &pos);
        static bool MouseWheelState(float x, float y);
        static bool MousePosition(const glm::vec2 &pos);
        static void UpdateMouse();

    private:
        void InitImGui(PlatformWindow *pWindow);
        void ImGui_ImplSurface_Shutdown();
        void ImGui_ImplSurface_NewFrame();
        void ImGui_ImplIRenderer_Draw();

    private:
        ShaderHandle m_VertexShader = 0;
        ShaderHandle m_PixelShader = 0;

        RenderBufferHandle m_VertexBuffer = 0;
        RenderBufferHandle m_IndexBuffer = 0;
        RenderBufferHandle m_VertexConstantBuffer = 0;
        RenderBufferHandle m_PixelConstantBuffer = 0;

        TextureHandle m_FontTexture = 0;
    };
}  // namespace lr