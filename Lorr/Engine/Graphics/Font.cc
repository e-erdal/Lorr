#include "Font.hh"

namespace Lorr
{
    using namespace msdfgen;

    static tiny_utf8::string g_Chars = " !\"#$%&'()*+,-./"
                                       "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ["
                                       "\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
                                       "ĞÜŞÖÇİ"
                                       "ğüşiöçıå"
                                       "©•";

    static FreetypeHandle *pFTHandle = 0;

    constexpr i32 kThreadCount = 4;
    constexpr u64 kMCGMultiplier = 6364136223846793005;

    void Font::Init(const Identifier &ident, FontDesc *pDesc, FontData *pData)
    {
        using namespace msdf_atlas;

        LOG_TRACE("Generating MTSDF atlas, {}<thread:{}>", ident, kThreadCount);

        pFTHandle = initializeFreetype();
        m_pHandle = loadFontData(pFTHandle, pData->TTFData.GetData(), pData->TTFData.GetSize());

        Charset charset;
        for (const auto &code : g_Chars) charset.add(code);

        std::vector<msdf_atlas::GlyphGeometry> glyphs;
        m_pGeometry = new FontGeometry(&glyphs);
        m_pGeometry->loadCharset(m_pHandle, 1, charset, true, true);

        //* Atlas generation *//
        /// Configure atlas
        TightAtlasPacker packer;
        packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::MULTIPLE_OF_FOUR_SQUARE);
        packer.setScale(pDesc->SizePX);
        packer.setPixelRange(8.f);
        packer.setPadding(0);
        packer.setMiterLimit(2.f);

        /// Generate atlas
        int glyphLeft = packer.pack(&glyphs[0], glyphs.size());
        if (glyphLeft > 0)
        {
            LOG_ERROR("A glyph couldn't fit inside atlas!");
            return;
        }

        int w, h;
        packer.getDimensions(w, h);
        Workload(
            [&](int i, int threadNo) -> bool {
                u64 glyphSeed = (kMCGMultiplier * (glyphSeed ^ i) + 1442695040888963407ull) * !!glyphSeed;
                glyphs[i].edgeColoring(msdfgen::edgeColoringByDistance, 3, glyphSeed);
                return true;
            },
            glyphs.size())
            .finish(kThreadCount);

        msdf_atlas::ImmediateAtlasGenerator<float, 4, mtsdfGenerator, BitmapAtlasStorage<byte, 4>> generator(w, h);
        GeneratorAttributes generatorAttrib;
        generatorAttrib.scanlinePass = true;
        generatorAttrib.config.overlapSupport = true;

        generator.setAttributes(generatorAttrib);
        generator.setThreadCount(kThreadCount);
        generator.generate(&glyphs[0], glyphs.size());

        msdfgen::BitmapConstRef<byte, 4> bitmap = generator.atlasStorage();

        /// Write atlas data into a Texture2D
        TextureDesc desc;
        TextureData data;
        size_t dataSize = 4 * bitmap.width * bitmap.height;
        data.Data = (u8 *)malloc(dataSize);
        for (int y = 0; y < bitmap.height; ++y) memcpy(&data.Data[4 * bitmap.width * y], bitmap(0, bitmap.height - y - 1), 4 * bitmap.width);
        data.DataSize = dataSize;
        data.Width = bitmap.width;
        data.Height = bitmap.height;

        m_Texture = Texture::Create("font", &desc, &data);

        LOG_TRACE("Created new atlas, {}<{}, {}>", ident, data.Width, data.Height);

        //* Load glyph info *//
        for (auto &glyph : glyphs)
        {
            GlyphInfo info;

            //* Calculate Texture Coordinates *//
            double l, b, r, t;
            glyph.getQuadAtlasBounds(l, b, r, t);
            double invB = m_Texture->GetHeight() - b;
            double invT = m_Texture->GetHeight() - t;

            l = 1.f / m_Texture->GetWidth() * l;
            b = 1.f / m_Texture->GetHeight() * invB;
            r = 1.f / m_Texture->GetWidth() * r;
            t = 1.f / m_Texture->GetHeight() * invT;

            info.TextureCoord = glm::vec4(l, t, r, b);  // V coord flip

            //* Calculate bounding box metrics *//
            glyph.getQuadPlaneBounds(l, b, r, t);
            info.BoundingBox = glm::vec4(l, b, r, t);

            info.Index = glyph.getIndex();
            info.Advance = glyph.getAdvance();

            m_Chars[glyph.getCodepoint()] = info;
        }

        for (auto &kerning : m_pGeometry->getKerning())
        {
            m_Kerning[std::make_pair<i32, i32>((i32)kerning.first.first, (i32)kerning.first.second)] = (float)kerning.second;
        }

        m_PixelRangle = packer.getPixelRange();
        m_SizePx = pDesc->SizePX;
    }

    void Font::ParseToMemory(FontData *pOutData, BufferStream &outBuf)
    {
    }

    void Font::AlignAll(const tiny_utf8::string &text, std::vector<TextLine> &outLines, float &outPixelRange, glm::vec2 &outSize, size_t maxWidth)
    {
        outPixelRange = m_PixelRangle;
        const msdfgen::FontMetrics fontMetrics = m_pGeometry->getMetrics();
        float fsScale = 1.0 / (fontMetrics.ascenderY - fontMetrics.descenderY);

        TextLine line;
        glm::vec2 pen = {};
        i32 beforeIndex = 0;  // Charset index of c[i - 1]
        for (int i = 0; i < text.length(); i++)
        {
            char32_t c = text[i];

            if (c == '\n')  // New line, expand height with lineHeight
            {
                pen.x = 0;
                pen.y += fsScale * fontMetrics.lineHeight;
                outSize.y = pen.y;

                outLines.push_back(line);
                line.Chars.clear();
                continue;
            }
            else if (c == '\t')
            {
                pen.x += 5.0 - fmod(pen.x, 5.0);
                continue;
            }
            else if (pen.x == 0 && c == ' ')  // Ignore whitespace at the start of line
            {
                continue;
            }

            auto glyphIt = m_Chars.find(c);
            if (glyphIt == m_Chars.end()) glyphIt = m_Chars.find('\n');
            const auto &glyph = glyphIt->second;

            float kerning = 0;
            if (i > 0)
            {
                auto kerningIt = m_Kerning.find(std::make_pair(beforeIndex, glyph.Index));
                if (kerningIt != m_Kerning.end()) kerning = kerningIt->second;
            }
            beforeIndex = glyph.Index;

            RenderableChar info;
            info.Position = glm::vec2(pen.x + glyph.BoundingBox.x, (-glyph.BoundingBox.w + fontMetrics.lineHeight) * fsScale + pen.y);
            info.Size = glm::vec2(glyph.BoundingBox.z - glyph.BoundingBox.x, glyph.BoundingBox.w - glyph.BoundingBox.y) * fsScale;
            info.UV = glyph.TextureCoord;
            line.Chars.push_back(info);

            pen.x += (glyph.Advance + kerning) * fsScale;
            line.Width = pen.x;

            outSize.x = fmax(outSize.x, pen.x);
        }

        outLines.push_back(line);
        line.Chars.clear();
    }

}  // namespace Lorr