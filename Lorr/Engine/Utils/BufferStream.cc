#include "BufferStream.hh"

#include "FileStream.hh"

#include <zlib.h>

#define ZLIB_PADDING_BYTES (1024 * 5)

u8 *lr::BufferStream::zlibDeflateToMemory(u8 *input, int sizeBytes, u32 *outDataSize)
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

u8 *lr::BufferStream::zLibInflateToMemory(u8 *input, unsigned int compressedSize, unsigned int decompressedSize)
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

void lr::BufferStream::operator=(FileStream &fs)
{
    u8 *pData = fs.ReadAll<u8>(); // This allocates extra memory, extremely wasteful so -> // TODO
    Reset(pData, fs.Size());
    free(pData);
}