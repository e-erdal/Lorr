//@TYPE v
cbuffer MVP : register(b0) { matrix prMat; };

struct VertexInput {
  float3 Pos : POSITION;
  float3 Normal : NORMAL;
};

struct PixelInput {
  float4 Pos : SV_POSITION;
  float3 Normal : NORMAL0;
};

PixelInput main(VertexInput input) {
  PixelInput output;
	output.Pos = mul(output.Pos, prMat);
	output.Normal = input.Normal;
  return output;
}

//@TYPE f
struct PixelInput {
  float4 Pos : SV_POSITION;
  float3 Normal : NORMAL0;
};

float main(PixelInput input) { return float4(1.0, 1.0, 1.0, 1.0); }