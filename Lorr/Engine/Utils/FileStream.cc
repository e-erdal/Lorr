#include "FileStream.hh"

#include "BufferStream.hh"

namespace lr
{
    FileStream::FileStream(const eastl::string &path, bool write)
    {
        if (write)
        {
            m_File = fopen(path.c_str(), "wb");
        }
        else
        {
            m_File = fopen(path.c_str(), "rb");
        }

        GetSize();
    }

    void FileStream::Reopen(const eastl::string &path, bool write)
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

        GetSize();
    }

    void FileStream::Close()
    {
        fclose(m_File);
    }

    void FileStream::WritePtr(const u8 *t, size_t size)
    {
        fwrite(t, 1, size, m_File);
    }

    void FileStream::WriteString(const eastl::string &val)
    {
        fwrite(val.c_str(), 1, val.length(), m_File);
    }

    void FileStream::GetSize()
    {
        if (m_FileSize > 0) return;

        fseek(m_File, 0, SEEK_END);
        m_FileSize = ftell(m_File);
        rewind(m_File);
    }

}  // namespace lr