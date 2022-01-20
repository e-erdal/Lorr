cbuffer ___ : register(b0) 
{
    matrix CameraMatrix;
    float2 ChunkPosition;
};

struct VSInput
{
    uint BlockData : BLENDINDICES0;
    float PosY     : PSIZE0;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    uint VertexID   : BLENDINDICES0;
};

PSInput VSMain(VSInput input, uint vertexID : SV_VertexID) 
{
    vertexID = vertexID % 4;

    float x = ChunkPosition.x + float((input.BlockData >> 24) & 0xFF); // 0xFF000000
    float z = ChunkPosition.y + float((input.BlockData >> 16) & 0xFF); // 0x00FF0000

    PSInput vsOut;
    vsOut.Position = mul(float4(x, input.PosY, z, 1.0), CameraMatrix);
    vsOut.VertexID = vertexID;

    return vsOut;
}