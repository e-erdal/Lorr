cbuffer ___ : register(b0) 
{
    matrix CameraMatrix;
};

struct VSInput
{
    float3 Pos          : POSITION;
    float2 TexCoords    : TEXCOORD0;
};

struct PSInput
{
    float4 Position     : SV_POSITION;
    float4 RawPosition  : NORMAL0;
    float2 TexCoords    : TEXCOORD0;
};

PSInput VSMain(VSInput input) 
{
    PSInput vsOut;
    vsOut.Position = mul(float4(input.Pos, 1.0), CameraMatrix);
    vsOut.RawPosition = vsOut.Position;
    vsOut.TexCoords = input.TexCoords;

    return vsOut;
}