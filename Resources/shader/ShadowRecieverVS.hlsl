#include "ShadowRecieverHeader.hlsli"

VSOutput main( float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VSOutput output;
    float4 worldPos = mul(mWorld, pos);
    output.pos = mul(mView, worldPos);
    output.pos = mul(mProj, output.pos);
    output.uv = uv;
    output.normal = mul(mWorld, normal);

    output.posInLVP = mul(mLVP, worldPos);

    return output;
}