//
// Created on July 10th 2021 by e-erdal.
//

#pragma once

#include "Engine/Core/Window/PlatformWindow.hh"

#include "Engine/Graphics/Common/IRenderBuffer.hh"
#include "Engine/Graphics/Common/IShader.hh"
#include "Engine/Graphics/Common/ITexture.hh"

#include <imgui.h>

namespace Lorr
{
    class Engine;
    class ImGuiHandler
    {
    public:
        ~ImGuiHandler();

        void Init(Engine *pEngine);

        void BeginFrame();
        void EndFrame();

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
}  // namespace Lorr