//$TYPE p
cbuffer PCbuf : register(b0) { float lodLevel; }

struct PS_Input {
  float4 Pos : SV_POSITION;
  float2 UV : TEXCOORD0;
  float4 Color : COLOR0;
};

Texture2D texture0;
SamplerState sampler0;

float4 main(PS_Input input) : SV_TARGET {
  return input.Color * texture0.SampleLevel(sampler0, input.UV, lodLevel);
}