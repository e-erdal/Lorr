//
// Created on Tuesday 5th October 2021 by e-erdal
// Non-SDF font atlas handler.
//

#pragma once

#include <freetype-gl.h>
#include <tinyutf8/tinyutf8.h>

#include "Engine/Graphics/Common/ITexture.hh"
#include "Engine/Resource/IResource.hh"

namespace Lorr
{
    struct CharInfo
    {
        glm::vec2 Pos;
        glm::vec2 Size;
        glm::vec4 UV;
    };

    struct FontDesc
    {
        uint8_t SizePX = 0;
    };

    struct FontData
    {
        BufferStream TTFData{};
        tiny_utf8::string UsingChars = "";
    };

    class Font : public IResource<FontDesc, FontData>
    {
    public:
        void Init(const Identifier &ident, FontDesc *pDesc, FontData *pData) override;
        static void ParseToMemory(FontData *pOutData, BufferStream &outBuf);

        ftgl::texture_glyph_t *GetGlyph(char32_t target);
        float GetKerning(char32_t before, char32_t current);

        static constexpr ResourceType m_ResType = ResourceType::Font;

    public:
        TextureHandle GetTexture() const
        {
            return m_Texture;
        }

        ftgl::texture_font_t *GetHandle() const
        {
            return m_pHandle;
        }

    private:
        ftgl::texture_atlas_t *m_pAtlas = 0;
        ftgl::texture_font_t *m_pHandle = 0;

        TextureHandle m_Texture = 0;
    };

    namespace TextEngine
    {
        void CalculateSize(Font *pFont, glm::vec2 &outSize, const tiny_utf8::string &text, size_t maxWidth);
        void AlignAll(Font *pFont, const tiny_utf8::string &text, std::vector<CharInfo> &outInfo, glm::vec2 &outSize, size_t maxWidth);
    }  // namespace TextEngine

}  // namespace Lorr