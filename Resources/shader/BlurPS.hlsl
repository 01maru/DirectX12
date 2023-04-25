#include  "BlurHeader.hlsli"
Texture2D<float4> sceneTexture : register(t0); // シーンテクスチャ
sampler Sampler : register(s0);
float4 main(VSOutput input) : SV_TARGET
{
    float4 Color;

    // 基準テクセルからプラス方向に8テクセル、重み付きでサンプリング
    Color = weight[0].x * sceneTexture.Sample(Sampler, input.tex0.xy);
    Color += weight[0].y * sceneTexture.Sample(Sampler, input.tex1.xy);
    Color += weight[0].z * sceneTexture.Sample(Sampler, input.tex2.xy);
    Color += weight[0].w * sceneTexture.Sample(Sampler, input.tex3.xy);
    Color += weight[1].x * sceneTexture.Sample(Sampler, input.tex4.xy);
    Color += weight[1].y * sceneTexture.Sample(Sampler, input.tex5.xy);
    Color += weight[1].z * sceneTexture.Sample(Sampler, input.tex6.xy);
    Color += weight[1].w * sceneTexture.Sample(Sampler, input.tex7.xy);
    
    // 基準テクセルにマイナス方向に8テクセル、重み付きでサンプリング
    Color += weight[0].x * sceneTexture.Sample(Sampler, input.tex0.zw);
    Color += weight[0].y * sceneTexture.Sample(Sampler, input.tex1.zw);
    Color += weight[0].z * sceneTexture.Sample(Sampler, input.tex2.zw);
    Color += weight[0].w * sceneTexture.Sample(Sampler, input.tex3.zw);
    Color += weight[1].x * sceneTexture.Sample(Sampler, input.tex4.zw);
    Color += weight[1].y * sceneTexture.Sample(Sampler, input.tex5.zw);
    Color += weight[1].z * sceneTexture.Sample(Sampler, input.tex6.zw);
    Color += weight[1].w * sceneTexture.Sample(Sampler, input.tex7.zw);
    
    return float4(Color.xyz, 1.0f);
}