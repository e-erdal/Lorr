//$TYPE p
struct PixelInput {
  float4 Pos : SV_POSITION;
  float2 TexCoord : TEXCOORD;
  float4 Color : COLOR;
};

sampler sampler0;
Texture2D texture0;

half4 UpsampleTent(float2 uv, float2 texelSize, float4 sampleScale)
{
    float4 d = texelSize.xyxy * float4(1.0, 1.0, -1.0, 0.0) * sampleScale;

    half4 s;
    s  = texture0.SampleLevel(sampler0, uv - d.xy, 0.0);
    s += texture0.SampleLevel(sampler0, uv - d.wy, 0.0) * 2.0;
    s += texture0.SampleLevel(sampler0, uv - d.zy, 0.0);

    s += texture0.SampleLevel(sampler0, uv - d.zw, 0.0) * 2.0;
    s += texture0.SampleLevel(sampler0, uv,        0.0) * 4.0;
    s += texture0.SampleLevel(sampler0, uv - d.xw, 0.0) * 2.0;

    s += texture0.SampleLevel(sampler0, uv - d.zy, 0.0);
    s += texture0.SampleLevel(sampler0, uv - d.wy, 0.0) * 2.0;
    s += texture0.SampleLevel(sampler0, uv - d.xy, 0.0);

    return s * (1.0 / 16.0);
}

half4 Downsample(float2 uv, float2 texelSize)
{
    float4 d = texelSize.xyxy * float4(-1.0, -1.0, 1.0, 1.0);

    half4 s;
    s  = texture0.SampleLevel(sampler0, uv + d.xy, 0.0);
    s += texture0.SampleLevel(sampler0, uv + d.zy, 0.0);
    s += texture0.SampleLevel(sampler0, uv + d.zw, 0.0);
    s += texture0.SampleLevel(sampler0, uv + d.xw, 0.0);

    return s * (1.0 / 4.0);
}

float4 main(PixelInput input) : SV_TARGET {
  return input.Color * texture0.SampleLevel(sampler0, input.TexCoord, 0.0);
}
