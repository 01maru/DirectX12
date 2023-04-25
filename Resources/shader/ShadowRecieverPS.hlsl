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
    float zInLVP = input.posInLVP.z;

    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        float2 shadowValue = g_shadowMap.Sample(g_sampler, shadowMapUV).xy;

        // まずこのピクセルが遮蔽されているか調べる。これは通常のデプスシャドウと同じ
        if (zInLVP > shadowValue.r && zInLVP <= 1.0f)
        {
            // 遮蔽されているなら、チェビシェフの不等式を利用して光が当たる確率を求める
            float depth_sq = shadowValue.x * shadowValue.x;

            // このグループの分散具合を求める
            // 分散が大きいほど、varianceの数値は大きくなる
            float variance = min(max(shadowValue.y - depth_sq, 0.0001f), 1.0f);

            // このピクセルのライトから見た深度値とシャドウマップの平均の深度値の差を求める
            float md = zInLVP - shadowValue.x;

            // 光が届く確率を求める
            float lit_factor = variance / (variance + md * md);

            // シャドウカラーを求める
            float3 shadowColor = texcolor.xyz * 0.5f;

            // 光が当たる確率を使って通常カラーとシャドウカラーを線形補完
            texcolor.xyz = lerp(shadowColor, texcolor.xyz, lit_factor);
        }

        //// step-3 シャドウマップに描き込まれているZ値と比較する
        //// 計算したUV座標を使って、シャドウマップから深度値をサンプリング
        //float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;
        //if (zInLVP > zInShadowMap)
        //{
        //    // 遮蔽されている
        //    texcolor.xyz *= 0.5f;
        //}
    }
    return texcolor;
}