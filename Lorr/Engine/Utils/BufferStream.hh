//
// Created on August 12th 2021 by e-erdal.
//

#pragma once

namespace lr
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
            LOG_TRACE("BufferStream allocated %llu bytes, total size is now %llu.", size, m_TotalSize);
        }

        void Deallocated(size_t size)
        {
            if (!m_Log) return;

            m_TotalSize -= size;
            LOG_TRACE("BufferStream deallocated %llu bytes, total size is now %llu.", size, m_TotalSize);
        }

    private:
        bool m_Log = false;
        size_t m_TotalSize = 0;
    };

    extern BufferStreamMemoyWatcher *g_pBSWatcher;

    class FileStream;
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

        BufferStream(eastl::vector<u8> &data)
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

        inline void Reset(eastl::vector<u8> &data)
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
        inline void AssignStringLen(const eastl::string &val)
        {
            auto lenT = sizeof(T);
            size_t len = val.length();

            Assign(len, lenT);

            if (val.length()) AssignPtr(val.c_str(), val.length());
        }

        inline void AssignString(const eastl::string &val)
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
        inline void InsertStringLen(const eastl::string &val)
        {
            auto lenT = sizeof(T);
            size_t len = val.length();

            Expand(val.length() + lenT);

            Assign(len, lenT);
            if (val.length()) AssignPtr(val.c_str(), val.length());
        }

        inline void InsertString(const eastl::string &val)
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
        inline eastl::string GetString(size_t dataLen = 0)
        {
            size_t strLen = (dataLen == 0 ? Get<T>() : dataLen);
            char *data = (char *)(m_Data + m_Offset);
            m_Offset += strLen;

            return eastl::string(data, strLen);
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

        void operator=(FileStream &fs);

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
        u8 *zlibDeflateToMemory(u8 *input, int sizeBytes, u32 *outDataSize);
        u8 *zLibInflateToMemory(u8 *input, unsigned int compressedSize, unsigned int decompressedSize);

    private:
        u8 *m_Data = 0;
        size_t m_DataLen = 0;

        uintptr_t m_Offset = 0;
    };  // namespace lr

}  // namespace lr