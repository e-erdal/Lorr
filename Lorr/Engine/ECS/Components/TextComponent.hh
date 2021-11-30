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
        Text(Font *pFont, const tiny_utf8::string &text, size_t maxWidth = 0)
        {
            SetText(pFont, text, maxWidth);
        }

        void SetText(Font *pFont, const tiny_utf8::string &text, size_t maxWidth = 0)
        {
            glm::vec2 a;
            pFont->AlignAll(text, m_Chars, a, maxWidth);
            m_Texture = pFont->GetTexture();
        }

        TextureHandle m_Texture;

        std::vector<RenderableChar> m_Chars;
    };

}  // namespace Lorr::Component