cbuffer MVP : register(b0) {
  matrix prMat;
};

struct VertexInput {
  float4 Pos : POSITION;
  float2 TexCoord : TEXCOORD0;
  float4 Color : COLOR0;
};

struct PixelInput {
  float4 Pos : SV_POSITION;
  float2 TexCoord : TEXCOORD;
  float4 Color : COLOR;
};

PixelInput VSMain(VertexInput input) {
  PixelInput output;

  output.Pos = mul(prMat, float4(input.Pos.xy, 0.0, 1.0));
  output.TexCoord = input.TexCoord;
  output.Color = input.Color;

  return output;
}