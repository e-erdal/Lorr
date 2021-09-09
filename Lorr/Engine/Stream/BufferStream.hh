//
// Created on August 12th 2021 by e-erdal.
//

#pragma once

#include <zlib.h>

#define ZLIB_PADDING_BYTES (1024 * 5)

namespace Lorr
{
    class BufferStream
    {
    public:
        BufferStream() = default;

        BufferStream(size_t dataLen)
        {
            Expand(dataLen);
            StartOver();
        }

        BufferStream(uint8_t *pData, size_t dataLen)
        {
            Expand(dataLen);
            Assign(pData, dataLen);
            StartOver();
        }

        BufferStream(std::vector<uint8_t> &data)
        {
            Expand(data.size());
            Assign(&data[0], data.size());
            StartOver();
        }

        ~BufferStream()
        {
            free(m_Data);
        }

    public:
        inline void Expand(size_t len)
        {
            m_DataLen += len;
            m_Data = _REALLOC(m_Data, m_DataLen);
            _ZEROM((m_Data + m_Offset), len);
        }

        // Copies input into buffer's data. Also increases offset.
        template<typename T>
        inline void Assign(T *pData, size_t dataLen)
        {
            assert(m_Data != NULL);            // Our data has to be valid
            assert(m_DataLen > 0);             // Our data has to be allocated
            assert((uint8_t *)pData != NULL);  // Data has to be vaild
            assert(m_DataLen >= dataLen);      // Input data cannot be larger than data we have

            memcpy(m_Data + m_Offset, (uint8_t *)pData, dataLen);
            m_Offset += dataLen;
        }

        inline void AssignString(const std::string &val)
        {
            Assign(val.data(), val.length());
        }

        // Copies input into buffer's data.
        template<typename T>
        inline void Insert(T &&pData, size_t dataLen = 0)
        {
            size_t size = (dataLen == 0 ? sizeof(T) : dataLen);

            Expand(size);

            memcpy(m_Data + m_Offset, (uint8_t *)pData, size);
            m_Offset += size;
        }

        inline void InsertString(const std::string &val)
        {
            size_t len = val.length();

            Expand(val.length());
            Assign(val.data(), val.length());
        }

        template<typename T>
        inline T &Get(size_t dataLen = 0)
        {
            T &data = *(T *)(m_Data + m_Offset);
            m_Offset += (dataLen == 0 ? sizeof(T) : dataLen);
            return data;
        }

        inline std::string GetString(size_t strLen)
        {
            char *data = (char *)(m_Data + m_Offset);
            m_Offset += strLen;

            return std::string(data, strLen);
        }

        inline void StartOver()
        {
            m_Offset = 0;
        }

        // UNTRUSTED
        // Decompress tool for buffer
        void Decompress(size_t compressedSize, size_t decompressedSize)
        {
            m_DataLen -= compressedSize;
            m_DataLen += decompressedSize;

            auto data = (m_Data + m_Offset);
            auto dcmp = zLibInflateToMemory(data, compressedSize, decompressedSize);

            m_Data = _REALLOC(m_Data, m_DataLen);

            memcpy(m_Data + m_Offset, dcmp, decompressedSize);
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

        inline const uintptr_t &GetOffset() const
        {
            return m_Offset;
        }

    private:
        static uint8_t *zlibDeflateToMemory(uint8_t *input, int sizeBytes, int *outDataSize)
        {
            z_stream strm;
            int ret;

            /* allocate deflate state */
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;

            ret = deflateInit(&strm, Z_BEST_COMPRESSION);
            if (ret != Z_OK) return nullptr;

            uint8_t *outData = (uint8_t *)malloc(sizeBytes + ZLIB_PADDING_BYTES);

            if (!outData) return nullptr;

            strm.avail_in = sizeBytes;
            strm.next_in = input;
            strm.avail_out = sizeBytes + ZLIB_PADDING_BYTES;
            strm.next_out = outData;

            ret = deflate(&strm, Z_FINISH);

            deflateEnd(&strm);
            *outDataSize = strm.total_out;
            return outData;
        }

        static uint8_t *zLibInflateToMemory(uint8_t *input, unsigned int compressedSize, unsigned int decompressedSize)
        {
            int ret;
            z_stream strm;

            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.avail_in = 0;
            strm.next_in = Z_NULL;

            ret = inflateInit(&strm);
            if (ret != Z_OK) return 0;

            uint8_t *dstData = (uint8_t *)malloc(decompressedSize);
            if (!dstData)
            {
                return nullptr;
            }

            strm.avail_in = compressedSize;
            strm.next_in = input;
            strm.avail_out = decompressedSize;
            strm.next_out = dstData;

            ret = inflate(&strm, Z_NO_FLUSH);
            if (!(ret == Z_OK || ret == Z_STREAM_END))
            {
                if (dstData) free(dstData);

                return nullptr;
            }

            (void)inflateEnd(&strm);

            return dstData;
        }

    private:
        uint8_t *m_Data = 0;
        size_t m_DataLen = 0;

        uintptr_t m_Offset = 0;
    };  // namespace Lorr

}  // namespace Lorr