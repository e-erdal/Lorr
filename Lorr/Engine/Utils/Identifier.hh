//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <iterator>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Lorr
{
    // Please note that this file does NOT own the string!
    class Identifier
    {
    public:
        // E.G file://Texture.png
        // E.G pkg://Texture.png
        // E.G http://Texture.png
        constexpr Identifier(std::string_view const &svUri)
        {
            assert(svUri.size() < 64 && "An identifier must be under 64 bytes!");

            // No constexpr std::copy :c
            for (size_t i = 0; i < svUri.size(); i++)
            {
                m_vString[i] = svUri[i];
            }

            auto uri = std::string_view(m_vString.data());

            constexpr std::string_view protoEnd("://");

            auto proto_end = uri.find_first_of(protoEnd);
            if (proto_end == std::string_view::npos)
            {
                return;
            }

            m_iProtoEnd = proto_end;
            m_iProtoSize = protoEnd.size();
        }

        constexpr Identifier(char *szUri): Identifier(std::string_view(szUri))
        {
        }
        constexpr Identifier(const char *szUri): Identifier(std::string_view(szUri))
        {
        }
        Identifier(std::string const &szUri): Identifier(std::string_view(szUri))
        {
        }

        constexpr std::string_view const Protocol() const
        {
            return std::string_view(m_vString.data(), m_iProtoEnd);
        }

        constexpr std::string_view const Path() const
        {
            return std::string_view(m_vString.data() + m_iProtoEnd + m_iProtoSize);
        }

        constexpr std::string_view const Raw() const
        {
            return std::string_view(m_vString.data());
        }

        constexpr operator const char *() const
        {
            return m_vString.data();
        }

        constexpr operator std::string_view() const
        {
            return Raw();
        }

        constexpr operator std::size_t() const
        {
            return std::hash<std::string_view>{}(Raw());
        }

        // Dumb piece of shit
        operator std::span<u8>() const
        {
            return std::span<u8>{(u8 *)m_vString.data(), m_vString.size()};
        }

        constexpr bool operator==(const Identifier &other) const
        {
            return m_vString == other.m_vString;
        }

    private:
        std::array<char, 64> m_vString{};

        u32 m_iProtoEnd = 0;
        u32 m_iProtoSize = 0;
    };

    constexpr Identifier kEmptyIdentifier = "engine://empty";
}  // namespace Lorr

// std::unordered_map support
template<>
struct std::hash<Lorr::Identifier>
{
    size_t operator()(const Lorr::Identifier &s) const noexcept
    {
        size_t h1 = std::hash<std::string_view>{}(s.Raw());

        return h1;
    }
};