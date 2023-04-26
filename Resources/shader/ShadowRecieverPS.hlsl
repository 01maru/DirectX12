#include "ShadowRecieverHeader.hlsli"

Texture2D<float4> g_albedo : register(t0);
Texture2D<float4> g_shadowMap : register(t1);  // シャドウマップ
sampler g_sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    //  textureのカラー
    float4 texcolor = g_albedo.Sample(g_sampler, input.uv);

    // ライトビュースクリーン空間からUV空間に座標変換しシャドウマップのUV計算
    float2 shadowMapUV = input.posInLVP.xy / input.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    // ライトビュースクリーン空間でのZ値
    float zInLVP = input.posInLVP.z;


    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        //  shadowMapの値出力
        float2 shadowValue = g_shadowMap.Sample(g_sampler, shadowMapUV).xy;

        // このピクセルが遮蔽されているか(Lposとworldposの距離が1000以下)
        if (zInLVP > shadowValue.x && zInLVP <= 1.0f)
        {
            float depth_sq = shadowValue.x * shadowValue.x;

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
    }

    return texcolor;
}