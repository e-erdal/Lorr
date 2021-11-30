//$TYPE v
cbuffer MVP : register(b0) { matrix prMat; }

struct VertexInput {
  float4 Pos      : POSITION;
  float3 Normal   : NORMAL0;
  float2 UV       : TEXCOORD0;
  float4 Color    : COLOR0;
};

struct PixelInput {
  float4 Pos      : SV_POSITION;
  float3 Normal   : NORMAL0;
  float2 UV       : TEXCOORD0;
  float4 Color    : COLOR0;
};

PixelInput main(VertexInput input) {
  PixelInput output;

	output.Pos = mul(input.Pos, prMat);
	output.Normal = input.Normal;
  output.UV = input.UV;
	output.Color = input.Color;

  return output;
}