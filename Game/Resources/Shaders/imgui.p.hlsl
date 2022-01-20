//$TYPE p
cbuffer PCbuf : register(b0) { float lodLevel; }

struct PS_Input {
  float4 Pos  : SV_POSITION;
  float2 UV   : TEXCOORD0;
  uint Color  : BLENDINDICES0;
};

Texture2D texture0    : register(t0);
SamplerState sampler0 : register(s0);

float4 PSMain(PS_Input input) : SV_TARGET {

  float a = float((input.Color >> 24) & 0xFF) / 255.0;
  float b = float((input.Color >> 16) & 0xFF) / 255.0;
  float g = float((input.Color >> 8 ) & 0xFF) / 255.0;
  float r =      ( input.Color        & 0xFF) / 255.0;
  return float4(r, g, b, a) * texture0.SampleLevel(sampler0, input.UV, lodLevel);
}