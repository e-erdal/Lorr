//
// Created on Tuesday 5th October 2021 by e-erdal
// Non-SDF font atlas handler.
//

#pragma once

#include <msdfgen.h>
#include <msdfgen-ext.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>

#include <tinyutf8/tinyutf8.h>

#include "Engine/Graphics/Common/BaseTexture.hh"
#include "Engine/Resource/IResource.hh"

namespace lr
{
    struct RenderableChar
    {
        glm::vec2 Position = {};
        glm::vec4 UV = {};
        glm::vec2 Size = {};
    };

    struct FontDesc
    {
        u8 SizePX = 0;
    };

    struct FontData
    {
        BufferStream TTFData = {};
        tiny_utf8::string UsingChars = "";
    };

    struct FontRenderBuffer
    {
        glm::vec4 RangePx = {};
    };

    enum class TextAlignment
    {
        Left,
        Right,
        Middle
    };

    struct TextLine
    {
        eastl::vector<RenderableChar> Chars;
        float Width;
    };

    class Font : public IResource<FontDesc, FontData>
    {
    public:
        void Init(const Identifier &ident, FontDesc *pDesc, FontData *pData) override;
        static void ParseToMemory(FontData *pOutData, BufferStream &outBuf);

        static constexpr ResourceType m_ResType = ResourceType::Font;

    public:
        TextureHandle GetTexture() const
        {
            return m_Texture;
        }

        float GetSizePx()
        {
            return m_SizePx;
        }

    private:
        msdfgen::FontHandle *m_pHandle = nullptr;
        msdf_atlas::FontGeometry *m_pGeometry = nullptr;

        struct GlyphInfo
        {
            glm::vec4 TextureCoord = {};
            glm::vec4 BoundingBox = {};

            float Advance = 0;
            i32 Index = 0;
        };
        eastl::unordered_map<char32_t, GlyphInfo> m_Chars;
        eastl::map<eastl::pair<i32, i32>, float> m_Kerning;

        TextureHandle m_Texture = 0;
        float m_PixelRangle = 0;
        float m_SizePx = 0;

    public:
        void AlignAll(const tiny_utf8::string &text, eastl::vector<TextLine> &outLines, float &outPixelRange, glm::vec2 &outSize, size_t maxWidth);
    };

}  // namespace lr