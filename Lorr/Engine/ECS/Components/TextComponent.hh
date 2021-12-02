//
// Created on Wednesday 6th October 2021 by e-erdal
//

#pragma once

#include "Engine/ECS/Entity.hh"

#include "Engine/Graphics/Font.hh"

namespace Lorr::Component
{
    struct Text
    {
        Text(Font *pFont, TextAlignment alignment, const tiny_utf8::string &text, size_t maxWidth = 0)
        {
            m_Alignment = alignment;

            SetText(pFont, text, maxWidth);
        }

        void SetText(Font *pFont, const tiny_utf8::string &text, size_t maxWidth = 0)
        {
            pFont->AlignAll(text, m_Lines, m_PixelRange, m_Size, maxWidth);
            m_Texture = pFont->GetTexture();
            m_SizePx = pFont->GetSizePx();
        }

        TextAlignment m_Alignment = TextAlignment::Left;

        float m_PixelRange;
        float m_SizePx;

        glm::vec2 m_Size;

        TextureHandle m_Texture;

        std::vector<TextLine> m_Lines;
    };

}  // namespace Lorr::Component