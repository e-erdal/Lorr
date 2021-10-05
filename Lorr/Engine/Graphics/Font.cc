#include "Font.hh"

#include <locale>
#include <codecvt>

namespace Lorr
{
    static tiny_utf8::string g_szASCII = " !\"#$%&'()*+,-./"
                                         "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ["
                                         "\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
                                         "ĞÜŞÖÇİ"
                                         "ğüşiöçı";

    static tiny_utf8::string GetNextWord(const tiny_utf8::string &str, uint32_t p1, uint32_t p2)
    {
        constexpr auto delimiters = U" \n\t";
        p1 = str.find_first_not_of(delimiters, p1);
        if (p1 == -1) return "";
        p2 = str.find_first_of(delimiters, p1);
        return str.substr(p1, (p2 - p1));
    }

    void Font::Init(const Identifier &ident, FontData *pData, uint8_t sizePX)
    {
        m_pAtlas = ftgl::texture_atlas_new(512, 512, 4);
        m_pHandle = ftgl::texture_font_new_from_memory(m_pAtlas, sizePX, pData->TTFData.GetData(), pData->TTFData.GetSize());
        ftgl::texture_font_load_glyphs(m_pHandle, g_szASCII.c_str());

        Texture2DData data;
        data.Width = 512;
        data.Height = 512;
        data.DataSize = m_pAtlas->width * m_pAtlas->height * m_pAtlas->depth;
        data.Data = m_pAtlas->data;

        m_pTexture = new Texture2D;
        m_pTexture->Init("", &data, 0);
    }

    void Font::ParseToMemory(FontData *pOutData, BufferStream &imageBuffer)
    {
    }

    ftgl::texture_glyph_t *Font::GetGlyph(char32_t target)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
        return ftgl::texture_font_get_glyph(m_pHandle, convert.to_bytes(target).c_str());
    }

    float Font::GetKerning(char32_t before, char32_t current)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
        return ftgl::texture_glyph_get_kerning(GetGlyph(before), convert.to_bytes(current).c_str());
    }

    void TextEngine::CalculateSize(Font *pFont, glm::vec2 &outSize, const tiny_utf8::string &text, size_t maxWidth)
    {
    }

    void TextEngine::AlignAll(Font *pFont, const tiny_utf8::string &text, std::vector<CharInfo> &outInfo, glm::vec2 &outSize, size_t maxWidth)
    {
        outInfo.clear();

        ftgl::texture_font_t *ftlFont = pFont->GetHandle();
        glm::vec2 pen{};
        float lineYDist = ftlFont->ascender - ftlFont->descender + ftlFont->linegap;
        float holyShit = ftlFont->height - ftlFont->size;
        float nuts = ftlFont->ascender + ftlFont->descender;

        outSize.y = lineYDist;

        for (int i = 0; i < text.length(); i++)
        {
            char32_t c = text[i];

            if (c == '\n')  // 1 - Check if we require new line
            {
                pen.x = 0;
                pen.y += lineYDist;
                outSize.y = pen.y;

                continue;
            }
            else if (pen.x == 0 && c == ' ')  // 2 - Always ignore the space at first
            {
                continue;
            }
            else if (maxWidth > 0 && i > 0 && c == ' ')  // 3 - Check if we need a width and process text
            {
                uint32_t wordWidth = 0;
                tiny_utf8::string word = GetNextWord(text, i, text.length());
                for (auto n : word)
                {
                    ftgl::texture_glyph_t *inf = pFont->GetGlyph(n);
                    if (!inf) inf = pFont->GetGlyph(n);

                    wordWidth += inf->advance_x + inf->offset_x;  // i have no idea how, but it works or does it?
                }

                // Check if sum of pen and word greater than width we require, basically "new line" logic up there
                if (pen.x + wordWidth > maxWidth)
                {
                    pen.y += lineYDist;
                    outSize.y = pen.y;

                    pen.x = 0;

                    continue;
                }
            }

            // Now we've done everything, let's place characters by their correct info

            ftgl::texture_glyph_t *glyph = pFont->GetGlyph(c);

            float kerning = 0.f;
            if (i > 0)
            {
                pen.x += (kerning = pFont->GetKerning(text[i - 1], c));
            }

            CharInfo info;
            info.Pos = { pen.x + glyph->offset_x, pen.y + nuts - glyph->offset_y + holyShit };
            info.Size = { glyph->width, glyph->height };
            info.UV = { glyph->s0, glyph->t0, glyph->s1, glyph->t1 };

            outInfo.push_back(info);

            pen.x += glyph->advance_x;
            outSize.x = fmax(outSize.x, pen.x);
        }
    }

}  // namespace Lorr