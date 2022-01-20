//
// Created on August 15th 2021 by e-erdal.
//

#pragma once

namespace lr
{
    class BufferStream;
    class FileStream
    {
    public:
        FileStream() = default;
        FileStream(const eastl::string &path, bool write);
        void Reopen(const eastl::string &path, bool write);

        void Close();

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

        void WritePtr(const u8 *t, size_t size = 0);
        void WriteString(const eastl::string &val);

    private:
        void GetSize();

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
        u32 m_FileSize = 0;
    };
}  // namespace lr