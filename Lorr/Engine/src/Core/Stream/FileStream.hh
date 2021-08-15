//
// Created on August 15th 2021 by e-erdal.
//

#pragma once

namespace Lorr
{
    class FileStream
    {
    public:
        FileStream() = default;

        FileStream( const std::string &path, bool write )
        {
            if ( write )
            {
                m_File = fopen( path.c_str(), "wb" );
            }
            else
            {
                m_File = fopen( path.c_str(), "rb" );
            }
        }

        ~FileStream()
        {
            fclose( m_File );
        }

        inline void Close()
        {
            fclose( m_File );
        }

        inline void GetSize()
        {
            fseek( m_File, 0, SEEK_END );
            m_FileSize = ftell( m_File );
            fseek( m_File, 0, SEEK_SET );
        }

        inline uint8_t *ReadAll()
        {
            GetSize();

            uint8_t *buffer = 0;
            fread( buffer, 1, m_FileSize, m_File );
            return buffer;
        }

        template<typename T = std::not_equal_to<uint8_t>>
        inline T *&ReadAll()
        {
            uint8_t *buffer = 0;
            fread( buffer, m_FileSize, 0, m_File );
            return (T) buffer;
        }

        template<typename T>
        inline void Write( const T &t, size_t size = 0 )
        {
            if ( size > 0 )
                fwrite( &t, 1, size, m_File );
            else
                fwrite( &t, 1, sizeof( T ), m_File );
        }

        inline void WriteString( const std::string &val )
        {
            fwrite( val.c_str(), 1, val.length(), m_File );
        }

    private:
        FILE *m_File = 0;
        long m_FileSize = 0;
    };
}  // namespace Lorr