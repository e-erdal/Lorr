//
// Created on August 12th 2021 by e-erdal.
//

#pragma once

#define _REALLOC( x, len ) (uint8_t *) realloc( (void *) x, len )
#define _MALLOC( len ) (uint8_t *) malloc( len )
#define _ZEROM( x, len ) memset( (void *) x, 0, len )

namespace Lorr
{
    class Buffer
    {
    public:
        Buffer() = default;

        Buffer( size_t DataLen )
        {
            Expand( DataLen );
        }

        Buffer( uint8_t *pData, size_t DataLen )
        {
            Expand( DataLen );
            Assign( pData, DataLen );
        }

        Buffer( std::vector<uint8_t> &Data )
        {
            Expand( Data.size() );
            Assign( &Data[0], Data.size() );
        }

    public:
        inline void Expand( size_t Len )
        {
            m_DataLen += Len;
            m_Data = _REALLOC( m_Data, m_DataLen );
            _ZEROM( m_Data, m_DataLen );
        }

        // Copies input into buffer's data. Also increases offset.
        template<typename T>
        inline void Assign( T *pData, size_t DataLen )
        {
            assert( m_Data != NULL );             // Our data has to be valid
            assert( m_DataLen > 0 );              // Our data has to be allocated
            assert( (uint8_t *) pData != NULL );  // Data has to be vaild
            assert( m_DataLen > DataLen );        // Input data cannot be larger than data we have

            memcpy( m_Data + m_Offset, (uint8_t *) pData, DataLen );
            m_Offset += DataLen;
        }

        inline void AssignString( const std::string &String )
        {
            size_t len = String.length();

            Assign( &len, 4 );
            Assign( String.data(), String.length() );
        }

        // Copies input into buffer's data.
        inline void Insert( void *pData, size_t DataLen )
        {
            Expand( DataLen );

            memcpy( m_Data + m_Offset, pData, DataLen );
            m_Offset += DataLen;
        }

        inline void InsertString( const std::string &String )
        {
            size_t len = String.length();

            Expand( String.length() + 4 );
            Assign( &len, 4 );
            Assign( String.data(), String.length() );
        }

    public:
        inline const uint8_t *GetData() const
        {
            return m_Data;
        }

        inline const size_t &GetSize() const
        {
            return m_DataLen;
        }

    private:
        uint8_t *m_Data = 0;
        size_t m_DataLen = 0;

        uintptr_t m_Offset = 0;
    };  // namespace Lorr

}  // namespace Lorr