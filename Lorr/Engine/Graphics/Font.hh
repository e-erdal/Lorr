//
// Created on Tuesday 5th October 2021 by e-erdal
// Non-SDF font atlas handler.
//

#pragma once

#include <msdfgen.h>
#include <msdfgen-ext.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>

#include <tinyutf8/tinyutf8.h>

#include "Engine/Graphics/Common/ITexture.hh"
#include "Engine/Resource/IResource.hh"

namespace Lorr
{
    struct RenderableChar
    {
        glm::vec4 Bounds = {};
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

    private:
        msdfgen::FontHandle *m_pHandle = nullptr;
        msdf_atlas::FontGeometry *m_pGeometry = nullptr;

        struct GlyphInfo
        {
            glm::ivec2 BoxSize = {};
            glm::vec4 TextureCoord = {};
            glm::vec4 BoundingBox = {};

            float Advance = 0;
            i32 Index = 0;
        };
        std::unordered_map<char32_t, GlyphInfo> m_Chars;

        TextureHandle m_Texture = 0;
        float m_PixelRangle = 0;

    public:
        void AlignAll(const tiny_utf8::string &text, std::vector<RenderableChar> &outChars, float &outPixelRange, glm::vec2 &outSize, size_t maxWidth);
    };

}  // namespace Lorr