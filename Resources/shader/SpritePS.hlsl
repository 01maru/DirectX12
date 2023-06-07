#include "SpriteHeader.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 output = float4(tex.Sample(smp,input.uv)) * color;

	//	“§‰ß’l0‚È‚ç•`‰æ‚µ‚È‚¢
	if (output.a == 0.0f) {
		discard;
	}

	return output;
}