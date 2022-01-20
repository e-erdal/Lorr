cbuffer Buf : register(b0) {
    float DistanceFactor;
};

struct PixelInput {
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float4 Color : COLOR;
};

sampler sampler0;
Texture2D texture0;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float4 PSMain(PixelInput input) : SV_TARGET {
    float3 sampled = texture0.Sample(sampler0, input.TexCoord).xyz;
    float signedDist = DistanceFactor * (median(sampled.x, sampled.y, sampled.z) - 0.5);
    float opacity = clamp(signedDist + 0.5, 0.0, 1.0);
    return float4(input.Color.xyz, opacity);
}
