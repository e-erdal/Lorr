//$TYPE c
Texture2D<float4> InTexture 		: register(t0);
RWTexture2D<float4> OutTexture0 	: register(u0);
SamplerState tapFilter 				: register(s0);

float4 Downsample(float2 uv, float2 texelSize, float lod) {
	half4 A = InTexture.SampleLevel(tapFilter, uv + texelSize * float2(-1.0, -1.0), lod);
    half4 B = InTexture.SampleLevel(tapFilter, uv + texelSize * float2( 0.0, -1.0), lod);
    half4 C = InTexture.SampleLevel(tapFilter, uv + texelSize * float2( 1.0, -1.0), lod);
    half4 D = InTexture.SampleLevel(tapFilter, uv + texelSize * float2(-0.5, -0.5), lod);

    half4 E = InTexture.SampleLevel(tapFilter, uv + texelSize * float2( 0.5, -0.5), lod);
    half4 F = InTexture.SampleLevel(tapFilter, uv + texelSize * float2(-1.0,  0.0), lod);
    half4 G = InTexture.SampleLevel(tapFilter, uv                                 , lod);
    half4 H = InTexture.SampleLevel(tapFilter, uv + texelSize * float2( 1.0,  0.0), lod);

    half4 I = InTexture.SampleLevel(tapFilter, uv + texelSize * float2(-0.5,  0.5), lod);
    half4 J = InTexture.SampleLevel(tapFilter, uv + texelSize * float2( 0.5,  0.5), lod);
    half4 K = InTexture.SampleLevel(tapFilter, uv + texelSize * float2(-1.0,  1.0), lod);
    half4 L = InTexture.SampleLevel(tapFilter, uv + texelSize * float2( 0.0,  1.0), lod);

    half4 M = InTexture.SampleLevel(tapFilter, uv + texelSize * float2( 1.0,  1.0), lod);
	
	const half2 div = (1.0 / 4.0) * half2(0.5, 0.125);

	half4 o = (D + E + I + J) * div.x;
    o += (A + B + G + F) * div.y;
    o += (B + C + H + G) * div.y;
    o += (F + G + L + K) * div.y;
    o += (G + H + M + L) * div.y;

    return float4(o.xyzw);
}


[numthreads(8, 8, 1)]
void main(uint groupIndex : SV_GroupIndex, uint3 groupThreadID : SV_GroupThreadID, uint2 dispatchID : SV_DispatchThreadID) {
	float2 srcSize;
	InTexture.GetDimensions(srcSize.x, srcSize.y);

	const float lod = 0;
	float2 uv = dispatchID / srcSize;
	float2 texelSize = 1 / srcSize;

	const float gamma = 2.2;
  
    // exposure tone mapping
    float3 mapped = float3(1.0, 1.0, 1.0) - exp(-Downsample(uv, texelSize, lod).xyz * 0.1);
    // gamma correction 
    mapped = pow(mapped, float3(1.0 / gamma, 1.0 / gamma, 1.0 / gamma));

	OutTexture0[dispatchID] = float4(mapped, 1.0);
}