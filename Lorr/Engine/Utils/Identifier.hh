//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include <cassert>
#include <span>

namespace lr
{
    // Please note that this file does NOT own the string!
    class Identifier
    {
    public:
        // E.G file://Texture.png
        // E.G pkg://Texture.png
        // E.G http://Texture.png
        constexpr Identifier(eastl::string_view const &svUri)
        {
            assert(svUri.size() < 64 && "An identifier must be under 64 bytes!");

            // No constexpr std::copy :c
            for (size_t i = 0; i < svUri.size(); i++)
            {
                m_vString[i] = svUri[i];
            }

            /// No EASTL SV, has some issues with this code
            auto uri = std::string_view(m_vString.data());
            constexpr std::string_view kProtoEnd = "://";

            auto proto_end = uri.find_first_of(kProtoEnd);
            if (proto_end == eastl::string_view::npos)
            {
                return;
            }

            m_iProtoEnd = proto_end;
            m_iProtoSize = kProtoEnd.size();
        }

        constexpr Identifier(char *szUri) : Identifier(eastl::string_view(szUri))
        {
        }
        constexpr Identifier(const char *szUri) : Identifier(eastl::string_view(szUri))
        {
        }
        Identifier(eastl::string const &szUri) : Identifier(eastl::string_view(szUri))
        {
        }
        Identifier(std::string const &szUri) : Identifier(eastl::string_view(szUri.data(), szUri.size()))
        {
        }

        constexpr eastl::string_view const Protocol() const
        {
            return eastl::string_view(m_vString.data(), m_iProtoEnd);
        }

        constexpr eastl::string_view const Path() const
        {
            return eastl::string_view(m_vString.data() + m_iProtoEnd + m_iProtoSize);
        }

        constexpr eastl::string_view const Raw() const
        {
            return eastl::string_view(m_vString.data());
        }

        constexpr operator char *() const
        {
            return (char *)m_vString.data();
        }

        constexpr operator const char *() const
        {
            return m_vString.data();
        }

        constexpr operator eastl::string_view() const
        {
            return Raw();
        }

        constexpr operator std::size_t() const
        {
            return eastl::hash<eastl::string_view>{}(Raw());
        }

        // Dumb piece of shit
        operator std::span<u8>() const
        {
            return std::span<u8>{ (u8 *)m_vString.data(), m_vString.size() };
        }

        constexpr bool operator==(const Identifier &other) const
        {
            return m_vString == other.m_vString;
        }

    private:
        eastl::array<char, 64> m_vString{};

        u32 m_iProtoEnd = 0;
        u32 m_iProtoSize = 0;
    };

    constexpr Identifier kEmptyIdentifier = "engine://empty";
}  // namespace lr

// eastl::unordered_map support
template<>
struct eastl::hash<lr::Identifier>
{
    size_t operator()(const lr::Identifier &s) const noexcept
    {
        size_t h1 = eastl::hash<eastl::string_view>{}(s.Raw());

        return h1;
    }
};