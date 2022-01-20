//$TYPE p
struct PixelInput {
  float4 Pos : SV_POSITION;
  float2 TexCoord : TEXCOORD;
  float4 Color : COLOR;
};

sampler sampler0;
Texture2D texture0;

float4 PSMain(PixelInput input) : SV_TARGET {
  return input.Color * texture0.SampleLevel(sampler0, input.TexCoord, 0.0);
}
