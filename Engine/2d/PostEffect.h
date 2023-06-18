#pragma once
#include "ViewPortScissorRect.h"
#include "VertIdxBuff.h"
#include "ConstBuff.h"
#include "Texture.h"
#include <vector>
#include <cstdint>

class GPipeline;
namespace CBuff {
	struct CBuffColorMaterial;
}

//	ぺらポリゴン
class PostEffect :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	int32_t texNum_ = 2;
	std::vector<Texture> texture_;
	
#pragma region ConstBuff

	CBuff::CBuffColorMaterial* cMaterialMap_ = nullptr;
	ConstBuff material_;

	ConstBuff weight_;
	
#pragma endregion

	std::vector<float> weights_;

	std::vector<ScreenVertex> vertices_;
	std::vector<uint16_t> indices_;

	D3D12_RESOURCE_BARRIER barrierDesc_{};

	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };

	ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	//	ビューポートシザー矩形
	ViewPortScissorRect viewPortSciRect_;

	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_{};
	ComPtr<ID3D12Resource> depthBuff_;
public:
	void Initialize(int32_t width, int32_t height, float weight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	void Setting();
	void DrawLuminnce();
	void Draw(bool xBlur, bool yBlur, bool shadow, int32_t handle1 = -1);
	void SetColor(const Vector4D& color);

	ID3D12Resource* GetTextureBuff(int32_t index = 0) { return texture_[index].GetResourceBuff(); }
	ID3D12Resource** GetTextureBuffPtr(int32_t index = 0) { return texture_[index].GetResourceBuffAddress(); }
	int32_t GetTextureNum() { return texNum_; }
	D3D12_RESOURCE_BARRIER& GetResouceBarrier() { return barrierDesc_; }
	ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap_.Get(); }
	ID3D12DescriptorHeap* GetDSVHeap() { return dsvHeap_.Get(); }
	Texture GetTexture() { return texture_[0]; }
private:
	void SetVertices() override;
};

