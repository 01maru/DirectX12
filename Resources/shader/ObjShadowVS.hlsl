#include "ObjShadowHeader.hlsli"

VSOutput main(float4 pos : POSITION)
{
	VSOutput output;
	output.svpos = mul(matworld, pos);
	float3 worldPos = output.svpos;
	output.svpos = mul(matview, output.svpos);

	output.depth.x = length(worldPos - lightPos) / 100.0f;
	output.depth.y = output.depth.x * output.depth.x;

	return output;
}