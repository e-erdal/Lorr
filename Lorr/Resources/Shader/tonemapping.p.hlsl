//$TYPE p
struct PixelInput {
	float4 Pos 	: SV_POSITION;
	float2 UV 	: TEXCOORD;
};

cbuffer inBuf   : register(b0) {
    float4 Exposure;
};

Texture2D texture0      : register(t0);
Texture2D texture1      : register(t1);
SamplerState sampler0   : register(s0);
SamplerState sampler1   : register(s1);

float3 ApplyThreshold(float3 rgb, float threshold) {
    return max(rgb - float3(threshold, threshold, threshold), float3(0.0, 0.0, 0.0));
}

float3 Chromatic(float2 uv, float2 direction, float3 distortion) {
    return float3(
        texture0.SampleLevel(sampler0, uv + direction * distortion.x, 0.0).x,
        texture0.SampleLevel(sampler0, uv + direction * distortion.y, 0.0).y,
        texture0.SampleLevel(sampler0, uv + direction * distortion.z, 0.0).z
        );
}

float4 main(PixelInput input) : SV_TARGET {
    float2 srcSize;
    texture0.GetDimensions(srcSize.x, srcSize.y);
    float2 texelSize = 1 / srcSize;
    float3 distortion = float3(-texelSize.x * 1.5, 0.0, texelSize.x * 1.5);

    float4 color = texture0.SampleLevel(sampler0, input.UV, 0.0);

    float2 texCoord = float2(1.0, 1.0) - input.UV;
    float2 ghostVec = (float2(0.5, 0.5) - texCoord) * 0.30;
    float2 direction = normalize(ghostVec);
    float2 haloVec = float2(0.5, 0.5) - texCoord;

    float4 res = float4(0, 0, 0, 0);
    for (int i = 0; i < 3; i++) {
        float2 offset = frac(texCoord + ghostVec * float(i));

        float weight = length(float2(0.5, 0.5) - offset) / length(float2(0.5, 0.5));
        weight = pow(1.0 - weight, 10.0);

        res += texture0.SampleLevel(sampler0, offset, 0.0) * weight;
    }

    //res *= texture1.SampleLevel(sampler1, length(float2(0.5, 0.5) - texCoord) / length(float2(0.5, 0.5)), 0.0);

	return color + res;
}