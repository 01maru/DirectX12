#include "ObjShadowHeader.hlsli"

VSOutput main(float4 pos : POSITION)
{
	VSOutput output;
	output.svpos = mul(matworld, pos);
	output.svpos = mul(matview, output.svpos);

	float3 worldPos = mul(matworld, pos);
	output.depth.x = length(worldPos - lightPos) / 1000.0f;
	output.depth.y = output.depth.x * output.depth.x;

	return output;
}