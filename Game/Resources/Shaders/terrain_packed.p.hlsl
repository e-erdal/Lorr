cbuffer ___ : register(b0)
{
    float3 LightDirection;
    float AmbientColor;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    uint VertexID   : BLENDINDICES0;
};

static float2 kTexCoords[] = { 
    { 1.0, 0.0 },
    { 0.0, 0.0 },
    { 0.0, 1.0 },
    { 1.0, 1.0 } 
};

SamplerState AtlasSampler   : register(s0);
Texture2D AtlasHandle       : register(t0);

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 sampledColor = AtlasHandle.Sample(AtlasSampler, kTexCoords[input.VertexID]).xyz;
    // float3 ambient = AmbientColor * sampledColor;
    // float diff = max(dot(LightDirection, kNormals[face - 1]), 0.0);
    // float3 diffuse = diff * sampledColor;

	return float4(sampledColor, 1.0);
}