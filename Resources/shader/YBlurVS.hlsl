#include  "BlurHeader.hlsli"
Texture2D<float4> sceneTexture : register(t0); // シーンテクスチャ
sampler Sampler : register(s0);
VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD0)
{
    // step-14 縦ブラー用の頂点シェーダーを実装
    VSOutput output;

    // 座標変換
    output.pos = pos;

    // テクスチャサイズを取得
    float2 texSize;
    float level;
    sceneTexture.GetDimensions(0, texSize.x, texSize.y, level);

    // 基準テクセルのUV座標を記録
    float2 tex = uv;

    // 基準テクセルからV座標を+1テクセルずらすためのオフセットを計算する
    output.tex0.xy = float2(0.0f, 1.0f / texSize.y);

    // 基準テクセルからV座標を+3テクセルずらすためのオフセットを計算する
    output.tex1.xy = float2(0.0f, 3.0f / texSize.y);

    // 基準テクセルからV座標を+5テクセルずらすためのオフセットを計算する
    output.tex2.xy = float2(0.0f, 5.0f / texSize.y);

    // 基準テクセルからV座標を+7テクセルずらすためのオフセットを計算する
    output.tex3.xy = float2(0.0f, 7.0f / texSize.y);

    // 基準テクセルからV座標を+9テクセルずらすためのオフセットを計算する
    output.tex4.xy = float2(0.0f, 9.0f / texSize.y);

    // 基準テクセルからV座標を+11テクセルずらすためのオフセットを計算する
    output.tex5.xy = float2(0.0f, 11.0f / texSize.y);

    // 基準テクセルからV座標を+13テクセルずらすためのオフセットを計算する
    output.tex6.xy = float2(0.0f, 13.0f / texSize.y);

    // 基準テクセルからV座標を+15テクセルずらすためのオフセットを計算する
    output.tex7.xy = float2(0.0f, 15.0f / texSize.y);

    // オフセットに-1を掛けてマイナス方向のオフセットも計算する
    output.tex0.zw = output.tex0.xy * -1.0f;
    output.tex1.zw = output.tex1.xy * -1.0f;
    output.tex2.zw = output.tex2.xy * -1.0f;
    output.tex3.zw = output.tex3.xy * -1.0f;
    output.tex4.zw = output.tex4.xy * -1.0f;
    output.tex5.zw = output.tex5.xy * -1.0f;
    output.tex6.zw = output.tex6.xy * -1.0f;
    output.tex7.zw = output.tex7.xy * -1.0f;

    // オフセットに基準テクセルのUV座標を足し算して、
    // 実際にサンプリングするUV座標に変換する
    output.tex0 += float4(tex, tex);
    output.tex1 += float4(tex, tex);
    output.tex2 += float4(tex, tex);
    output.tex3 += float4(tex, tex);
    output.tex4 += float4(tex, tex);
    output.tex5 += float4(tex, tex);
    output.tex6 += float4(tex, tex);
    output.tex7 += float4(tex, tex);
    return output;
}