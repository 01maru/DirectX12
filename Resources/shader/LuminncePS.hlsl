#include "ScreenHeader.hlsli"

Texture2D<float4> mainRenderTargetTexture : register(t0);
sampler Sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    // メインレンダリングターゲットからカラーをサンプリング
    float4 color = mainRenderTargetTexture.Sample(Sampler, input.uv);

    // サンプリングしたカラーの明るさを計算
    float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));

    clip(t - 1.0f);

    return color;
}