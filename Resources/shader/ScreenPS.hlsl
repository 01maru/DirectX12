#include "ScreenHeader.hlsli"

Texture2D<float4>tex : register(t0);
Texture2D<float4> g_bokeTexture_1 : register(t1);
Texture2D<float4> g_bokeTexture_2 : register(t2);
Texture2D<float4> g_bokeTexture_3 : register(t3);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 output = float4(tex.Sample(smp, input.uv));
	output += g_bokeTexture_1.Sample(smp, input.uv);
	//output += g_bokeTexture_2.Sample(smp, input.uv);
	//output += g_bokeTexture_3.Sample(smp, input.uv);
	//output /= 2.0f;
	output.a = 1.0f;

	//// step-1 基準テクセル+近傍8テクセルの平均を計算する
	//// 2.5テクセル分ずらすためのUV値を求める
	//float offsetU = 1.5f / 1280.0f;
	//float offsetV = 1.5f / 720.0f;

	//// 基準テクセルから右のテクセルのカラーをサンプリングする
	//output += tex.Sample(smp, input.uv + float2(offsetU, 0.0f));

	//// 基準テクセルから左のテクセルのカラーをサンプリングする
	//output += tex.Sample(smp, input.uv + float2(-offsetU, 0.0f));

	//// 基準テクセルから下のテクセルのカラーをサンプリングする
	//output += tex.Sample(smp, input.uv + float2(0.0f, offsetV));

	//// 基準テクセルから上のテクセルのカラーをサンプリングする
	//output += tex.Sample(smp, input.uv + float2(0.0f, -offsetV));

	//// 基準テクセルから右下のテクセルのカラーをサンプリングする
	//output += tex.Sample(smp, input.uv + float2(offsetU, offsetV));

	//// 基準テクセルから右上のテクセルのカラーをサンプリングする
	//output += tex.Sample(smp, input.uv + float2(offsetU, -offsetV));

	//// 基準テクセルから左下のテクセルのカラーをサンプリングする
	//output += tex.Sample(smp, input.uv + float2(-offsetU, offsetV));

	//// 基準テクセルから左上のテクセルのカラーをサンプリングする
	//output += tex.Sample(smp, input.uv + float2(-offsetU, -offsetV));

	//// 基準テクセルと近傍8テクセルの平均なので9で除算する
	//output /= 9.0f;
	output *= color;
	//if (output.a == 0.0f) {
	//	discard;
	//}

	return output;
}