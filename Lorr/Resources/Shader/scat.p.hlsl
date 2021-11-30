//$TYPE p
cbuffer cb {
  // camera
  matrix mvp;
  float3 cameraPosition;
  float cameraHeight2;

  float atmosRadius, atmosRadius2;
  float earthRadius, earthRadius2;
  // 1 / (atmosRadius - earthRadius)
  float invAtmosHeight;

  // float KrSun, KmSun;
  float3 lightDirection;
  float3 betaR, betaM;
  float3 sunIntensity;
  float g;
  float samples;
};

struct input {
  float4 colorR : COLOR0;
  float4 colorM : COLOR1;
  float4 intersect : COLOR2;
  float3 texCoord0 : TEXCOORD0;
  float2 texCoord1 : TEXCOORD1;
};

#define PI 3.141592653

float4 tonemapping(float4 color) {
  color.x = color.x < 1.413f ? pow(color.x * 0.38317f, 1.0f / 2.2f)
                             : 1.0f - exp(-color.x);
  color.y = color.y < 1.413f ? pow(color.y * 0.38317f, 1.0f / 2.2f)
                             : 1.0f - exp(-color.y);
  color.z = color.z < 1.413f ? pow(color.z * 0.38317f, 1.0f / 2.2f)
                             : 1.0f - exp(-color.z);
  color.w = color.w;

  return color;
}

float xx(float cosA, float g) {
  float g2 = g * g;

  float a = 1.0 - g2;
  float b = 2.0 + g2;

  float c = 1.0 + cosA * cosA;
  float d = pow(1.0 + g2 - 2.0 * g * cosA, 1.5);

  return (3.0 / 2.0) * a / b * c / d;
}

Texture2D earthGround;
sampler textureSampler;

float4 main(input i) : SV_TARGET {
  float cosTheta = dot(normalize(i.texCoord0), -lightDirection);

  float phaseM = 1.5f * ((1.0f - g * g) / (2.0f + g * g)) *
                 (1.0f + cosTheta * cosTheta) /
                 pow(1.0f + g * g - 2.0f * g * cosTheta, 1.5f);
  float phaseR = 3.0f / (16.0f * PI) * (1.0f + cosTheta * cosTheta);

  float4 color = phaseM * i.colorM + i.colorR * phaseR;

  return float4(color.xyz, color.z);
  // if (i.intersect.x == 0.0f)
  //     return tonemapping(float4(color.xyz, 1.0f));
  // else
  //     return tonemapping(float4(color.xyz *
  //     earthGround.Sample(textureSampler, i.texCoord1).xyz, 1.0f));
  // return tonemapping(float4(color.xyz, color.z));
}