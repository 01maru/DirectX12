#include "ObjShadowHeader.hlsli"

VSOutput main(float4 pos : POSITION)
{
	VSOutput output;
	float3 worldPos = mul(matworld, pos);
	output.svpos = mul(matview, worldPos);

	output.depth.x = length(worldPos - lightPos) / 1000.0f;
	output.depth.y = output.depth.x * output.depth.x;

	return output;
}