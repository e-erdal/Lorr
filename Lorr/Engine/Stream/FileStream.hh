//
// Created on August 15th 2021 by e-erdal.
//

#pragma once

#include <fstream>
#include <streambuf>

namespace lr
{
    class FileStream
    {
    public:
        FileStream() = default;

        FileStream(const eastl::string &path, bool write)
        {
            if (write)
            {
                m_File = fopen(path.c_str(), "wb");
            }
            else
            {
                m_File = fopen(path.c_str(), "rb");
            }
        }

        void Reopen(const eastl::string &path, bool write)
        {
            if (IsOK()) Close();

            if (write)
            {
                m_File = fopen(path.c_str(), "wb");
            }
            else
            {
                m_File = fopen(path.c_str(), "rb");
            }
        }

        inline void Close()
        {
            fclose(m_File);
        }

        template<typename T>
        inline T *ReadAll()
        {
            GetSize();

            T *buffer = (T *)malloc(m_FileSize);
            fread(buffer, 1, m_FileSize, m_File);
            return buffer;
        }

        template<typename T>
        inline T *ReadPtr(size_t size = 0)
        {
            T *buffer;

            if (size == 0)
                fread(buffer, 1, sizeof(T), m_File);
            else
                fread(buffer, 1, size, m_File);

            return buffer;
        }

        template<typename T>
        inline T &Read(size_t size = 0)
        {
            T *buffer;
            fread(buffer, 1, (size == 0 ? sizeof(T) : size), m_File);
            return *buffer;
        }

        template<typename T>
        inline void Write(const T &t, size_t size = 0)
        {
            if (size > 0)
                fwrite(&t, 1, size, m_File);
            else
                fwrite(&t, 1, sizeof(T), m_File);
        }

        inline void WritePtr(const u8 *t, size_t size = 0)
        {
            fwrite(t, 1, size, m_File);
        }

        inline void WriteString(const eastl::string &val)
        {
            fwrite(val.c_str(), 1, val.length(), m_File);
        }

    private:
        inline void GetSize()
        {
            if (m_FileSize > 0) return;

            fseek(m_File, 0, SEEK_END);
            m_FileSize = ftell(m_File);
            rewind(m_File);
        }

    public:
        size_t Size()
        {
            return m_FileSize;
        }

        inline bool IsOK()
        {
            return m_File;
        }

    private:
        FILE *m_File = 0;
        long m_FileSize = 0;
    };
}  // namespace lr