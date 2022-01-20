cbuffer ___ : register(b0) 
{
    matrix CameraMatrix;
    float2 ChunkPosition;
};

struct VSInput
{
    float3 Pos          : POSITION;
    float2 TexCoords    : TEXCOORD0;
};

struct PSInput
{
    float4 Position     : SV_POSITION;
    float2 TexCoords    : TEXCOORD0;
};

PSInput VSMain(VSInput input) 
{
    PSInput vsOut;
    vsOut.Position = mul(float4(ChunkPosition.x + input.Pos.x, input.Pos.y, ChunkPosition.y + input.Pos.z, 1.0), CameraMatrix);
    vsOut.TexCoords = input.TexCoords;

    return vsOut;
}