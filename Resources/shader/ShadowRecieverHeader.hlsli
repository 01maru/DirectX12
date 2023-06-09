// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    matrix mViewProj;
    matrix mWorld;
};

// ライトビュープロジェクション行列の定数バッファーを定義
cbuffer ShadowCb : register(b1)
{
    matrix mLVP;
    float3 lightPos;    // ライトの座標
};

struct VSOutput
{
    float4 pos : SV_POSITION;       // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL;         // 法線
    float2 uv : TEXCOORD0;          // UV座標
    float4 posInLVP : TEXCOORD1;    // ライトビュースクリーン空間でのピクセルの座標
};