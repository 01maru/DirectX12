#include "ShadowRecieverHeader.hlsli"

Texture2D<float4> g_albedo : register(t0);
Texture2D<float4> g_shadowMap : register(t1);  // シャドウマップ
sampler g_sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = g_albedo.Sample(g_sampler, input.uv);

    // ライトビュースクリーン空間からUV空間に座標変換
    float2 shadowMapUV = input.posInLVP.xy / input.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    // step-4 ライトビュースクリーン空間でのZ値を計算する
    float zInLVP = input.posInLVP.z / input.posInLVP.w;

    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        // step-3 シャドウマップに描き込まれているZ値と比較する
        // 計算したUV座標を使って、シャドウマップから深度値をサンプリング
        float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;
        if (zInLVP > zInShadowMap)
        {
            // 遮蔽されている
            texcolor.xyz *= 0.5f;
        }
    }
    return texcolor;
}