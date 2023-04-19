#include "ObjShadowHeader.hlsli"

float4 main(VSOutput input) : SV_TARGET0
{
	//return float4(0.2f, 0.2f, 0.2f, 1.0f);
	return float4(input.svpos.z, input.svpos.z, input.svpos.z, 1.0f);
}