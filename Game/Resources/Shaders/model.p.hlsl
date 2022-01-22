//$TYPE p
struct PixelInput {
  float4 Pos      : SV_POSITION;
  float3 Normal   : NORMAL0;
  float2 UV       : TEXCOORD0;
  float4 Color    : COLOR0;
};

Texture2D texture2d;
SamplerState samplerState;

float4 PSMain(PixelInput input) : SV_TARGET {
  float3 lightDir = float3(1, 0.5, 0);
  float3 color = texture2d.Sample(samplerState, input.UV).xyz * input.Color.xyz;
  
  float ambientStrength = 0.1;
  float3 ambient = ambientStrength * color;

  float d = max(dot(normalize(input.Normal), lightDir), 0);
  float3 diffuse = d * color;

  return float4(1.0, 0.0, 0.0, 1.0f);
}