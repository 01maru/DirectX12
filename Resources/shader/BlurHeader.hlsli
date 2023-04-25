struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 tex0 : TEXCOORD0;
    float4 tex1 : TEXCOORD1;
    float4 tex2 : TEXCOORD2;
    float4 tex3 : TEXCOORD3;
    float4 tex4 : TEXCOORD4;
    float4 tex5 : TEXCOORD5;
    float4 tex6 : TEXCOORD6;
    float4 tex7 : TEXCOORD7;
};

cbuffer cb : register(b0)
{
    float4 mulColor;    // èÊéZÉJÉâÅ[
};

cbuffer CBBlur : register(b1)
{
    float4 weight[2]; // èdÇ›
}

