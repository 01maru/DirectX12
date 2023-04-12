#include "ShadowRecieverHeader.hlsli"

Texture2D<float4> g_albedo : register(t0);
Texture2D<float4> g_shadowMap : register(t1);  // シャドウマップ
sampler g_sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float2 shadowMapUV = input.posInLVP.xy / input.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    float3 shadowMap = 1.0f;
    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        shadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV);
    }

    float4 color = g_albedo.Sample(g_sampler, input.uv);

    color.xyz *= shadowMap;

    return color;
}