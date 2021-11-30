//$TYPE c
Texture2D<float4> InTexture 		: register(t0);
RWTexture2D<float4> OutTexture0 	: register(u0);
SamplerState Sampler 				: register(s0);
cbuffer InBuffer 					: register(b0)
{
	float4 ScaleBias; // [scale, bias, lod, 0]
};

[numthreads(8, 8, 1)]
void main(uint groupIndex : SV_GroupIndex, uint3 groupThreadID : SV_GroupThreadID, uint2 dispatchID : SV_DispatchThreadID) {
	float2 srcSize;
	InTexture.GetDimensions(srcSize.x, srcSize.y);
	float2 uv = dispatchID / srcSize + 0.5 / srcSize;

	const float kernel[9] = {
			0.077847, 0.123317, 0.077847,
			0.123317, 0.195346, 0.123317,
			0.077847, 0.123317, 0.077847,
		};

	const float2 scale = 2.0 / srcSize;
	const float2 offsets[9] = {
			float2(-1.0, -1.0), float2( 0.0, -1.0), float2( 1.0, -1.0),
			float2(-1.0,  0.0), float2( 0.0,  0.0), float2( 1.0,  0.0),
			float2(-1.0,  1.0), float2( 0.0,  1.0), float2( 1.0,  1.0),
		};

	float4 color = float4(0, 0, 0, 0);
	for (int i = 0; i < 9; ++i) {
		color += InTexture.SampleLevel(Sampler, uv + offsets[i] * scale, ScaleBias.z) * kernel[i];
	}

	OutTexture0[dispatchID] = color;
}