#include "ObjShadowHeader.hlsli"

VSOutput main(float4 pos : POSITION) : SV_POSITION
{
	VSOutput output;
	output.svpos = mul(matworld, pos);
	output.svpos = mul(matview, output.svpos);
	return output;
}