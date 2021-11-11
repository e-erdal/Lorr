//
// Created on August 12th 2021 by e-erdal.
//

#pragma once

#include <zlib.h>

#define ZLIB_PADDING_BYTES (1024 * 5)

namespace Lorr
{
    class BufferStreamMemoyWatcher
    {
    public:
        BufferStreamMemoyWatcher(bool enable) : m_Log(enable)
        {
        }

        void Allocated(size_t size)
        {
            if (!m_Log) return;

            m_TotalSize += size;
            LOG_TRACE("BufferStream allocated {} bytes, total size is now {}.", size, m_TotalSize);
        }

        void Deallocated(size_t size)
        {
            if (!m_Log) return;
            
            m_TotalSize -= size;
            LOG_TRACE("BufferStream deallocated {} bytes, total size is now {}.", size, m_TotalSize);
        }

    private:
        bool m_Log = false;
        size_t m_TotalSize = 0;
    };

    extern BufferStreamMemoyWatcher *g_pBSWatcher;

    class BufferStream
    {
    public:
        BufferStream(){};

        BufferStream(size_t dataLen)
        {
            InsertZero(dataLen);
            StartOver();
        }

        BufferStream(u8 *pData, size_t dataLen)
        {
            Expand(dataLen);
            AssignPtr(pData, dataLen);
            StartOver();
        }

        BufferStream(std::vector<u8> &data)
        {
            Expand(data.size());
            AssignPtr(&data[0], data.size());
            StartOver();
        }

        ~BufferStream()
        {
            g_pBSWatcher->Deallocated(m_DataLen);
            SAFE_FREE(m_Data);
        }

    public:
        inline void Reset()
        {
            SAFE_FREE(m_Data);
            m_DataLen = 0;

            StartOver();
        }

        inline void Reset(size_t dataLen)
        {
            SAFE_FREE(m_Data);
            m_DataLen = 0;

            InsertZero(dataLen);
            StartOver();
        }

        inline void Reset(u8 *pData, size_t dataLen)
        {
            SAFE_FREE(m_Data);
            m_DataLen = 0;

            Expand(dataLen);
            AssignPtr(pData, dataLen);
            StartOver();
        }

        inline void Reset(std::vector<u8> &data)
        {
            SAFE_FREE(m_Data);
            m_DataLen = 0;

            Expand(data.size());
            AssignPtr(&data[0], data.size());
            StartOver();
        }

        inline void Reset(BufferStream &data)
        {
            SAFE_FREE(m_Data);
            m_DataLen = 0;

            Expand(data.m_DataLen);
            AssignPtr(data.m_Data, data.m_DataLen);

            StartOver();
        }

        inline u8 *GetOffsetPtr()
        {
            return (m_Data + m_Offset);
        }

        inline void Dump()
        {
            printf("========= BufferStream memory dump =========\n");

            for (size_t i = 0; i < m_DataLen; i++)
            {
                printf("%x ", m_Data[i]);
            }

            printf("\n");
        }

        inline void Seek(u8 seekTo, intptr_t pos)
        {
            switch (seekTo)
            {
                case SEEK_END: m_Offset = m_DataLen - pos; break;
                case SEEK_SET: m_Offset = pos; break;
                case SEEK_CUR: m_Offset += pos; break;
                default: break;
            }

            if (m_Offset < 0)
                m_Offset = 0;
            else if (m_Offset > m_DataLen)
                m_Offset = m_DataLen;
        }

        inline void Expand(size_t len)
        {
            m_DataLen += len;
            m_Data = _REALLOC(m_Data, m_DataLen);
            _ZEROM((m_Data + m_Offset), len);

            g_pBSWatcher->Allocated(len);
        }

        // Copies input into buffer's data. Also increases offset.
        template<typename T>
        inline void Assign(T &&pData, size_t dataLen = 0)
        {
            u8 *pRaw = (u8 *)&pData;
            size_t size = (dataLen == 0 ? sizeof(T) : dataLen);

            assert(m_Data != NULL);     // Our data has to be valid
            assert(m_DataLen > 0);      // Our data has to be allocated
            assert(pRaw != NULL);       // Data has to be vaild
            assert(m_DataLen >= size);  // Input data cannot be larger than data we have

            memcpy(m_Data + m_Offset, pRaw, size);
            m_Offset += size;
        }

        template<typename T>
        inline void AssignPtr(T *pData, size_t dataLen = 0)
        {
            u8 *pRaw = (u8 *)pData;
            size_t size = (dataLen == 0 ? sizeof(T) : dataLen);

            assert(m_Data != NULL);     // Our data has to be valid
            assert(m_DataLen > 0);      // Our data has to be allocated
            assert(pRaw != NULL);       // Data has to be vaild
            assert(m_DataLen >= size);  // Input data cannot be larger than data we have

            memcpy(m_Data + m_Offset, pRaw, size);
            m_Offset += size;
        }

        inline void AssignZero(size_t dataSize)
        {
            memset(m_Data + m_Offset, 0, dataSize);
            m_Offset += dataSize;
        }

        template<typename T>
        inline void AssignStringLen(const std::string &val)
        {
            auto lenT = sizeof(T);
            size_t len = val.length();

            Assign(len, lenT);

            if (val.length()) AssignPtr(val.c_str(), val.length());
        }

        inline void AssignString(const std::string &val)
        {
            if (val.length()) AssignPtr(val.c_str(), val.length());
        }

        inline void InsertZero(size_t dataLen)
        {
            Expand(dataLen);

            memset(m_Data + m_Offset, 0, dataLen);
            m_Offset += dataLen;
        }

        // Copies input into buffer's data.
        template<typename T>
        inline void Insert(T &&pData, size_t dataLen = 0)
        {
            size_t size = (dataLen == 0 ? sizeof(T) : dataLen);

            Expand(size);

            memcpy(m_Data + m_Offset, (u8 *)&pData, size);
            m_Offset += size;
        }

        inline void Insert(BufferStream &buf)
        {
            Expand(buf.m_DataLen);

            memcpy(m_Data + m_Offset, buf.m_Data, buf.m_DataLen);
            m_Offset += buf.m_DataLen;
        }

        template<typename T>
        inline void InsertPtr(T *pData, size_t dataLen)
        {
            size_t size = (dataLen == 0 ? sizeof(T) : dataLen * sizeof(T));

            Expand(size);

            memcpy(m_Data + m_Offset, (u8 *)pData, size);
            m_Offset += size;
        }

        // Inserts string into block with size
        template<typename T>
        inline void InsertStringLen(const std::string &val)
        {
            auto lenT = sizeof(T);
            size_t len = val.length();

            Expand(val.length() + lenT);

            Assign(len, lenT);
            if (val.length()) AssignPtr(val.c_str(), val.length());
        }

        inline void InsertString(const std::string &val)
        {
            size_t len = val.length();

            Expand(val.length());

            if (val.length()) AssignPtr(val.c_str(), val.length());
        }

        template<typename T>
        inline T &Get(size_t dataLen = 0)
        {
            T &data = *(T *)(m_Data + m_Offset);
            m_Offset += (dataLen == 0 ? sizeof(T) : dataLen);
            return data;
        }

        template<typename T>
        inline T *GetPtr(size_t dataLen)
        {
            T *data = (T *)(m_Data + m_Offset);
            m_Offset += (dataLen == 0 ? sizeof(T) : dataLen * sizeof(T));
            return data;
        }

        template<typename T>
        inline T *GetPtrNew(size_t dataLen)
        {
            size_t lenW = (dataLen == 0 ? sizeof(T) : dataLen * sizeof(T));
            T *data = (T *)malloc(lenW);
            memcpy(data, m_Data + m_Offset, lenW);
            m_Offset += lenW;
            return data;
        }

        template<typename T>
        inline std::string GetString(size_t dataLen = 0)
        {
            size_t strLen = (dataLen == 0 ? Get<T>() : dataLen);
            char *data = (char *)(m_Data + m_Offset);
            m_Offset += strLen;

            return std::string(data, strLen);
        }

        inline void StartOver()
        {
            m_Offset = 0;
        }

        // ! UNTRUSTED SECTION
        // Decompress tool for buffer
        void Decompress(size_t decompressedSize)
        {
            g_pBSWatcher->Deallocated(m_DataLen);

            u8 *dcompData = zLibInflateToMemory(m_Data, m_DataLen, decompressedSize);

            m_DataLen = decompressedSize;

            m_Data = _REALLOC(m_Data, m_DataLen);
            memcpy(m_Data, dcompData, m_DataLen);

            free(dcompData);

            g_pBSWatcher->Allocated(m_DataLen);
        }

        void Compress()
        {
            g_pBSWatcher->Deallocated(m_DataLen);

            u32 outSize = 0;
            u8 *compData = zlibDeflateToMemory(m_Data, m_DataLen, &outSize);

            m_DataLen = outSize;

            m_Data = _REALLOC(m_Data, m_DataLen);
            memcpy(m_Data, compData, m_DataLen);

            free(compData);

            g_pBSWatcher->Allocated(m_DataLen);
        }

    public:
        inline u8 *GetData()
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
        static u8 *zlibDeflateToMemory(u8 *input, int sizeBytes, u32 *outDataSize)
        {
            z_stream strm;
            int ret;

            /* allocate deflate state */
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;

            ret = deflateInit(&strm, Z_BEST_COMPRESSION);
            if (ret != Z_OK) return nullptr;

            u8 *outData = (u8 *)malloc(sizeBytes + ZLIB_PADDING_BYTES);

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

        static u8 *zLibInflateToMemory(u8 *input, unsigned int compressedSize, unsigned int decompressedSize)
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

            u8 *dstData = (u8 *)malloc(decompressedSize);
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
        u8 *m_Data = 0;
        size_t m_DataLen = 0;

        uintptr_t m_Offset = 0;
    };  // namespace Lorr

}  // namespace Lorr