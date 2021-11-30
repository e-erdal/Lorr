//$TYPE p
struct PixelInput {
	float4 Pos 	: SV_POSITION;
	float2 UV 	: TEXCOORD;
};

Texture2D texture0;
SamplerState sampler0;

float4 Downsample(float2 uv, float2 texelSize, float lod) {
	half4 A = texture0.SampleLevel(sampler0, uv + texelSize * float2(-1.0, -1.0), lod);
    half4 B = texture0.SampleLevel(sampler0, uv + texelSize * float2( 0.0, -1.0), lod);
    half4 C = texture0.SampleLevel(sampler0, uv + texelSize * float2( 1.0, -1.0), lod);
    half4 D = texture0.SampleLevel(sampler0, uv + texelSize * float2(-0.5, -0.5), lod);

    half4 E = texture0.SampleLevel(sampler0, uv + texelSize * float2( 0.5, -0.5), lod);
    half4 F = texture0.SampleLevel(sampler0, uv + texelSize * float2(-1.0,  0.0), lod);
    half4 G = texture0.SampleLevel(sampler0, uv                                 , lod);
    half4 H = texture0.SampleLevel(sampler0, uv + texelSize * float2( 1.0,  0.0), lod);

    half4 I = texture0.SampleLevel(sampler0, uv + texelSize * float2(-0.5,  0.5), lod);
    half4 J = texture0.SampleLevel(sampler0, uv + texelSize * float2( 0.5,  0.5), lod);
    half4 K = texture0.SampleLevel(sampler0, uv + texelSize * float2(-1.0,  1.0), lod);
    half4 L = texture0.SampleLevel(sampler0, uv + texelSize * float2( 0.0,  1.0), lod);

    half4 M = texture0.SampleLevel(sampler0, uv + texelSize * float2( 1.0,  1.0), lod);
	
	const half2 div = (1.0 / 4.0) * half2(0.5, 0.125);

	half4 o = (D + E + I + J) * div.x;
    o += (A + B + G + F) * div.y;
    o += (B + C + H + G) * div.y;
    o += (F + G + L + K) * div.y;
    o += (G + H + M + L) * div.y;

    return float4(o.xyzw);
}

float4 main(PixelInput input) : SV_TARGET {
	float2 srcSize;
	texture0.GetDimensions(srcSize.x, srcSize.y);
	float2 texelSize = 1 / srcSize;

	return Downsample(input.UV, texelSize, 0.0);
}