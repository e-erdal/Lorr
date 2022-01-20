//$TYPE v
struct VertexInput {
	float4 Pos 	: POSITION;
	float2 UV 	: TEXCOORD0;
};

struct PixelInput {
	float4 Pos 	: SV_POSITION;
	float2 UV	: TEXCOORD;
};

PixelInput VSMain(VertexInput input) {
	PixelInput output;

	output.Pos = float4(input.Pos.xyz, 1.0);
	output.UV = input.UV;

	return output;
}