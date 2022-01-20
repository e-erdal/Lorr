//
// Created on Friday 7th January 2022 by e-erdal
// Full: (Bin)ary (Ut)ility
// Collection of helper stuff that will make reading/writing binary databases A LOT easier
//

#pragma once

#include "Engine/Utils/BufferStream.hh"

namespace lr
{
    class BiNut
    {
    public:
        BiNut(const eastl::string &path);

    private:
        struct Element
        {
            enum class DataLenType : u8
            {
                Typeless,
                Typeless8,
                Typeless16,
                Typeless32,
                Typeless64,

                Integer8,
                Integer16,
                Integer32,
                Integer64,

                String8,
                String16,
                String32,

                Array,

            } m_Type;

            u8 *m_pData = nullptr;
        };
    };

}  // namespace lr