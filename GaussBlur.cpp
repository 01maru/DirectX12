#include "GaussBlur.h"
#include "DirectX.h"
#include "ConstBuffStruct.h"
#include <cassert>

void GaussBlur::Initialize(float weight)
{
#pragma region ConstBuff

	weight_.Initialize((sizeof(CBuff::CBufferBlurWeight) + 0xFF) & ~0xFF);

	weights_.resize(8);
	MyMath::CalcGaussianWeightsTable(
		weights_,        // 重みの格納先
		weight          // ボケ具合。この数値が大きくなるとボケが強くなる
	);

	CBuff::CBufferBlurWeight* mapWeight = nullptr;
	HRESULT result = weight_.GetResource()->Map(0, nullptr, (void**)&mapWeight);	//	マッピング
	mapWeight->weight[0] = Vector4D(weights_[0], weights_[1], weights_[2], weights_[3]);
	mapWeight->weight[1] = Vector4D(weights_[4], weights_[5], weights_[6], weights_[7]);
	assert(SUCCEEDED(result));
	weight_.GetResource()->Unmap(0, nullptr);

#pragma endregion

}

void GaussBlur::Draw()
{
	MyDirectX* dx = MyDirectX::GetInstance();
	dx->PrevPostEffect(&postEffect_[0]);

	originPE_->Draw(true, false, false);

	dx->PostEffectDraw(&postEffect_[0]);

	dx->PrevPostEffect(&postEffect_[1]);

	postEffect_[0].Draw(false, true, false);

	dx->PostEffectDraw(&postEffect_[1]);
}
