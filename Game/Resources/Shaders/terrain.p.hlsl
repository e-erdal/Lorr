cbuffer ___ : register(b0)
{
    float3 LightDirection;
    float AmbientColor;
};

struct PSInput
{
    float4 Position     : SV_POSITION;
    float4 RawPosition  : NORMAL0;
    float2 TexCoords    : TEXCOORD0;
};

SamplerState AtlasSampler   : register(s0);
Texture2D AtlasHandle       : register(t0);

float4 PSMain(PSInput input) : SV_TARGET
{
    float depth = input.RawPosition.z / input.RawPosition.w;

    float4 sampledColor = AtlasHandle.Sample(AtlasSampler, input.TexCoords);

    // float3 ambient = AmbientColor * sampledColor;
    // float diff = max(dot(LightDirection, kNormals[face - 1]), 0.0);
    // float3 diffuse = diff * sampledColor;

	return sampledColor;
}