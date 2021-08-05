cbuffer cb { matrix projection, view, model, normalM; }

struct Input {
  float4 position : POSITION;
  float3 normal : NORMAL;
  float4 color : COLOR;
};

struct Output {
  float4 position : SV_POSITION;
  float3 normal : NORMAL;
  float4 color : COLOR;
};

Output main(Input input) {
  Output output;

  // Change the position vector to be 4 units for proper matrix calculations.
  input.position.w = 1.0f;
  output.position = mul(mul(mul(input.position, model), view), projection);
  output.normal = mul(float4(input.normal, 1.0f), normalM).xyz;
  output.color = input.color;

  return output;
}