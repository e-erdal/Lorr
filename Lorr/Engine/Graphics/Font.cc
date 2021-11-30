#include "Font.hh"

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
        m_pGeometry->loadCharset(m_pHandle, 1, charset);

        //* Atlas generation *//
        /// Configure atlas
        TightAtlasPacker packer;
        packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::MULTIPLE_OF_FOUR_SQUARE);
        packer.setScale(40.f);
        packer.setPixelRange(4.f);
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

            info.TextureCoord = glm::vec4(l, t, r, b);

            //* Calculate bounding box metrics *//
            glyph.getQuadPlaneBounds(l, b, r, t);
            info.BoundingBox = glm::vec4(l, b, r, t);

            glyph.getBoxSize(info.BoxSize.x, info.BoxSize.y);

            info.Index = glyph.getIndex();
            info.Advance = glyph.getAdvance();

            m_Chars[glyph.getCodepoint()] = info;
        }
    }

    void Font::ParseToMemory(FontData *pOutData, BufferStream &outBuf)
    {
    }

    void Font::AlignAll(const tiny_utf8::string &text, std::vector<RenderableChar> &outChars, glm::vec2 &outSize, size_t maxWidth)
    {
        const msdfgen::FontMetrics fontMetrics = m_pGeometry->getMetrics();
        double fsScale = 1.0 / (fontMetrics.ascenderY - fontMetrics.descenderY);

        glm::vec2 pen = {};

        for (int i = 0; i < text.length(); i++)
        {
            char32_t c = text[i];

            if (c == '\n')  // New line, expand height with lineHeight
            {
                pen.x = 0;
                pen.y += fsScale * fontMetrics.lineHeight;
                outSize.y = pen.y;

                continue;
            }
            else if (pen.x == 0 && c == ' ')  // Ignore whitespace at the start of line
                continue;

            auto glyphIt = m_Chars.find(c);
            if (glyphIt == m_Chars.end()) glyphIt = m_Chars.find('\n');
            const auto &glyph = glyphIt->second;

            glm::vec4 planeBounds = glyph.BoundingBox;
            planeBounds *= fsScale;
            planeBounds.x += pen.x;
            planeBounds.z += pen.x;
            planeBounds.y += pen.y;
            planeBounds.w += pen.y;

            RenderableChar info;
            info.Bounds = planeBounds;
            info.Size = glyph.BoxSize;
            info.UV = glyph.TextureCoord;
            outChars.push_back(info);

            pen.x += fsScale * glyph.Advance;
            outSize.x = fmax(outSize.x, pen.x);
        }
    }

}  // namespace Lorr