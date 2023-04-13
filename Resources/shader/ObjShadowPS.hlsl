#include "ObjShadowHeader.hlsli"

float4 main(VSOutput output) : SV_TARGET0
{
	return float4(0.2f, 0.2f, 0.2f, 1.0f);
	//return float4(output.svpos.z, output.svpos.z, output.svpos.z, 1.0f);
}