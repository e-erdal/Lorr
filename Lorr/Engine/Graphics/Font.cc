#include "Font.hh"

#include <locale>
#include <codecvt>

namespace Lorr
{
    using namespace msdfgen;

    static tiny_utf8::string g_Chars = " !\"#$%&'()*+,-./"
                                       "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ["
                                       "\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
                                       "ĞÜŞÖÇİ"
                                       "ğüşiöçı"
                                       "©•";

    static FreetypeHandle *pFTHandle = 0;

    static tiny_utf8::string GetNextWord(const tiny_utf8::string &str, u32 p1, u32 p2)
    {
        constexpr auto delimiters = U" \n\t";
        p1 = str.find_first_not_of(delimiters, p1);
        if (p1 == -1) return "";
        p2 = str.find_first_of(delimiters, p1);
        return str.substr(p1, (p2 - p1));
    }

    constexpr i32 threadCount = 4;

    void Font::Init(const Identifier &ident, FontDesc *pDesc, FontData *pData)
    {
        using namespace msdf_atlas;

        LOG_TRACE("Generating MTSDF atlas, {}<thread:{}>", ident, threadCount);

        pFTHandle = initializeFreetype();
        m_pHandle = loadFontData(pFTHandle, pData->TTFData.GetData(), pData->TTFData.GetSize());

        Charset charset;
        for (const auto &code : g_Chars) charset.add(code);

        FontGeometry geometry(&m_Glyphs);
        geometry.loadCharset(m_pHandle, 1, charset, false, true);

        //* Atlas generation
        // Config atlas
        TightAtlasPacker packer;
        packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::MULTIPLE_OF_FOUR_SQUARE);
        packer.setScale(40.f);
        packer.setPixelRange(2.f);
        packer.setPadding(0);
        packer.setMiterLimit(2.f);

        // Generate atlas
        int glyphLeft = packer.pack(&m_Glyphs[0], m_Glyphs.size());
        if (glyphLeft > 0)
        {
            LOG_ERROR("A glyph couldn't fit inside atlas!");
            return;
        }

        int w, h;

        packer.getDimensions(w, h);

        constexpr u32 glyphSeed = 594561;

        Workload(
            [this](int i, int threadNo) -> bool {
                u64 glyphSeed = (6364136223846793005ull * (glyphSeed ^ i) + 1442695040888963407ull) * !!glyphSeed;
                m_Glyphs[i].edgeColoring(msdfgen::edgeColoringSimple, 3, glyphSeed);
                return true;
            },
            m_Glyphs.size())
            .finish(threadCount);

        msdf_atlas::ImmediateAtlasGenerator<float, 4, mtsdfGenerator, BitmapAtlasStorage<byte, 4>> generator(w, h);
        GeneratorAttributes generatorAttrib;
        generatorAttrib.scanlinePass = true;
        generatorAttrib.config.overlapSupport = true;

        generator.setAttributes(generatorAttrib);
        generator.setThreadCount(threadCount);
        generator.generate(&m_Glyphs[0], m_Glyphs.size());

        msdfgen::BitmapConstRef<byte, 4> bitmap = generator.atlasStorage();

        TextureDesc desc;
        TextureData data;

        size_t dataSize = 4 * bitmap.width * bitmap.height;
        data.Data = (u8 *)malloc(dataSize);
        for (int y = 0; y < bitmap.height; ++y) memcpy(&data.Data[4 * bitmap.width * y], bitmap(0, bitmap.height - y - 1), 4 * bitmap.width);

        data.DataSize = dataSize;
        data.Width = bitmap.width;
        data.Height = bitmap.height;

        m_Texture = Texture::Create("font", &desc, &data);

        LOG_TRACE("Created new atlas, {}", ident);
    }

    void Font::ParseToMemory(FontData *pOutData, BufferStream &outBuf)
    {
    }

    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;

    // ftgl::texture_glyph_t *Font::GetGlyph(char32_t target)
    // {
    //     return ftgl::texture_font_get_glyph(m_pHandle, convert.to_bytes(target).c_str());
    // }

    float Font::GetKerning(char32_t before, char32_t current)
    {
        // return ftgl::texture_glyph_get_kerning(GetGlyph(before), convert.to_bytes(current).c_str());
        return 0;
    }

    void TextEngine::CalculateSize(Font *pFont, glm::vec2 &outSize, const tiny_utf8::string &text, size_t maxWidth)
    {
    }

    void TextEngine::AlignAll(Font *pFont, const tiny_utf8::string &text, std::vector<CharInfo> &outInfo, glm::vec2 &outSize, size_t maxWidth)
    {
        outInfo.clear();

        // ftgl::texture_font_t *ftlFont = pFont->GetHandle();
        // glm::vec2 pen{};
        // float lineYDist = ftlFont->ascender - ftlFont->descender + ftlFont->linegap;
        // float holyShit = ftlFont->height - ftlFont->size;
        // float nuts = ftlFont->ascender + ftlFont->descender;

        // outSize.y = lineYDist;

        // for (int i = 0; i < text.length(); i++)
        // {
        //     char32_t c = text[i];

        //     if (c == '\n')  // 1 - Check if we require new line
        //     {
        //         pen.x = 0;
        //         pen.y += lineYDist;
        //         outSize.y = pen.y;

        //         continue;
        //     }
        //     else if (pen.x == 0 && c == ' ')  // 2 - Always ignore the space at first
        //     {
        //         continue;
        //     }
        //     else if (maxWidth > 0 && i > 0 && c == ' ')  // 3 - Check if we need a width and process text
        //     {
        //         u32 wordWidth = 0;
        //         tiny_utf8::string word = GetNextWord(text, i, text.length());
        //         for (auto n : word)
        //         {
        //             ftgl::texture_glyph_t *inf = pFont->GetGlyph(n);
        //             if (!inf) inf = pFont->GetGlyph(n);

        //             wordWidth += inf->advance_x + inf->offset_x;  // i have no idea how, but it works or does it?
        //         }

        //         // Check if sum of pen and word greater than width we require, basically "new line" logic up there
        //         if (pen.x + wordWidth > maxWidth)
        //         {
        //             pen.y += lineYDist;
        //             outSize.y = pen.y;

        //             pen.x = 0;

        //             continue;
        //         }
        //     }

        //     // Now we've done everything, let's place characters by their correct info

        //     ftgl::texture_glyph_t *glyph = pFont->GetGlyph(c);

        //     float kerning = 0.f;
        //     if (i > 0 && text[i - 1] > 32)
        //     {
        //         pen.x += (kerning = pFont->GetKerning(text[i - 1], c));
        //     }

        //     CharInfo info;
        //     info.Pos = { pen.x + glyph->offset_x, pen.y + nuts - glyph->offset_y + holyShit };
        //     info.Size = { glyph->width, glyph->height };
        //     info.UV = { glyph->s0, glyph->t0, glyph->s1, glyph->t1 };

        //     outInfo.push_back(info);

        //     pen.x += glyph->advance_x;
        //     outSize.x = fmax(outSize.x, pen.x);
        // }
    }

}  // namespace Lorr