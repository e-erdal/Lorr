//$TYPE p
struct PixelInput {
	float4 Pos 	: SV_POSITION;
	float2 UV	: TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET {
	return float4(input.Pos.xyz, 1);
}