//
// Created on Tuesday 7th December 2021 by e-erdal
// ImGui like GUI system, cause i love ImGui
//

#pragma once

#include "Engine/App/Engine.hh"

namespace lr::GUI
{
    struct Vertex
    {
        glm::vec2 Pos;
        glm::vec2 UV;
        u32 Color;
    };

    struct DrawData
    {
        u32 VertexCount = 0;
        u32 IndexCount = 0;
    };

    struct ConstantBuffer
    {
        glm::mat4 Matrix;
        float EdgeRadius = 0;  // Rounded panel edges
        /// Padding(3F)
    };

    struct Panel
    {
        glm::vec3 m_Pos = {};
        glm::vec2 m_Size = {};

        DrawData *GetDrawData(TextureHandle texture);

        /// Add a rect with texture(placeholder if null) into a panel.
        /// A panel must be initialized before calling this function.
        /// This method uses VertexBatcher way of indexed draws, containing GLM calculations which is very expensive.
        void AddRect(const glm::vec2 &pos, const glm::vec2 &size, u32 color, TextureHandle texture = nullptr);

        /// Prepares a font that will be used in next "Text Related" widgets, (button, label, etc...)
        // TODO: Font also can be SDF but SDF font rendering is quite expensive for now.
        void SetNextFont(const Identifier &fontIdent, TextAlignment alignment);

        /// Signed distacne field based font. Might be quite expensive for GUI.
        void AddTextSDF(const tiny_utf8::string &text, u32 maxWidth);

        /// Pre-initialized non-SDF, atlas based font.
        void AddText(const tiny_utf8::string &text, u32 maxWidth);
    };

    struct Context
    {
        /// Panel handler section ///
        std::vector<Panel> m_Panels;
        Panel *m_pCurrentPanel = nullptr;

        /// Renderer data section ///
        std::vector<Vertex> m_Vertices;
        Vertex *m_pVertPtr = nullptr;
        std::unordered_map<TextureHandle, DrawData> m_DrawData;

        RenderBufferHandle m_ConstantBuffer = nullptr;
        RenderBufferHandle m_VertexBuffer = nullptr;
        RenderBufferHandle m_IndexBuffer = nullptr;

        ShaderProgram *m_pBaseShaderProgram = nullptr;

        void AllocVertices(u32 size);
    };

    void Init();

    /// Widgets ///
    bool BeginPanel(const Identifier &ident, const glm::ivec2 &pos, const glm::ivec2 &size, const std::string &title);
    void EndPanel();

    /// Rendering ///
    void BeginFrame();
    void Render();
    void EndFrame();

}  // namespace lr::GUI