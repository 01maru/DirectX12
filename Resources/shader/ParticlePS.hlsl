#include "Particle.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	//float4 texcolor = float4(tex.Sample(smp,input.uv));
	//float3 light = normalize(float3(1,-1,1));
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3;
	float4 output = float4(tex.Sample(smp,input.uv)) * color;
	if (output.a <= 0.0f) {
		discard;
	}
	return output;
	//return float4(input.normal,1);
	//return float4(1,1,1,1);
}