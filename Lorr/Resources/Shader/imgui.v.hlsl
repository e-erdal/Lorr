//$TYPE v
cbuffer VP : register(b0) { matrix prMat; }

struct VS_Input {
  float2 Pos : POSITION;
  float2 UV : TEXCOORD0;
  float4 Color : COLOR0;
};

struct PS_Input {
  float4 Pos : SV_POSITION;
  float2 UV : TEXCOORD0;
  float4 Color : COLOR0;
};

PS_Input main(VS_Input input) {
  PS_Input output;
  output.Pos = mul(prMat, float4(input.Pos.xy, 0.f, 1.f));
  output.Color = input.Color;
  output.UV = input.UV;

  return output;
}