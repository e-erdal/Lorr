//
// Created on July 7th 2021 by e-erdal.
//

#pragma once

#define BitFlags( NAME )                                                                                                                                                 \
    inline NAME operator|( NAME a, NAME b )                                                                                                                              \
    {                                                                                                                                                                    \
        return static_cast<NAME>( static_cast<int>( a ) | static_cast<int>( b ) );                                                                                       \
    }                                                                                                                                                                    \
    inline NAME operator|( NAME a, int b )                                                                                                                               \
    {                                                                                                                                                                    \
        return static_cast<NAME>( static_cast<int>( a ) | static_cast<int>( b ) );                                                                                       \
    }                                                                                                                                                                    \
    inline NAME operator|( int a, NAME b )                                                                                                                               \
    {                                                                                                                                                                    \
        return static_cast<NAME>( static_cast<int>( a ) | static_cast<int>( b ) );                                                                                       \
    }                                                                                                                                                                    \
    inline int operator&( NAME a, NAME b )                                                                                                                               \
    {                                                                                                                                                                    \
        return static_cast<int>( static_cast<int>( a ) & static_cast<int>( b ) );                                                                                        \
    }                                                                                                                                                                    \
    inline int operator&( NAME a, int b )                                                                                                                                \
    {                                                                                                                                                                    \
        return static_cast<int>( static_cast<int>( a ) & static_cast<int>( b ) );                                                                                        \
    }                                                                                                                                                                    \
    inline int operator&( int a, NAME b )                                                                                                                                \
    {                                                                                                                                                                    \
        return static_cast<int>( static_cast<int>( a ) & static_cast<int>( b ) );                                                                                        \
    }                                                                                                                                                                    \
    inline NAME operator~( NAME a )                                                                                                                                      \
    {                                                                                                                                                                    \
        return static_cast<NAME>( ~static_cast<int>( a ) );                                                                                                              \
    }                                                                                                                                                                    \
    inline NAME &operator|=( NAME &a, NAME b )                                                                                                                           \
    {                                                                                                                                                                    \
        return a = a | b;                                                                                                                                                \
    }                                                                                                                                                                    \
    inline NAME &operator|=( NAME &a, int b )                                                                                                                            \
    {                                                                                                                                                                    \
        return a = a | b;                                                                                                                                                \
    }                                                                                                                                                                    \
    inline NAME &operator&=( NAME &a, NAME b )                                                                                                                           \
    {                                                                                                                                                                    \
        return a = static_cast<NAME>( a & b );                                                                                                                           \
    }                                                                                                                                                                    \
    inline NAME &operator&=( NAME &a, int b )                                                                                                                            \
    {                                                                                                                                                                    \
        return a = static_cast<NAME>( a & b );                                                                                                                           \
    }
