#pragma once
#include <d3d12.h>
#include "ViewPort.h"
#include "VertIdxBuff.h"
#include "Texture.h"
#include <vector>
#pragma comment(lib, "d3d12.lib")

//	ぺらポリゴン
class PostEffect :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	int texNum = 2;
	std::vector<Texture> texture;

	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	ComPtr<ID3D12Resource> material;
	struct ConstBufferWeight {
		Vector4D weight[2];
	};
	ComPtr<ID3D12Resource> weightMaterial;
	
	const int NUM_WEIGHTS = 8;
	float weights[8];

	std::vector<ScreenVertex> vertices;
	UINT indexSize = 6;
	unsigned short indices[6] = {};

	D3D12_RESOURCE_BARRIER barrierDesc;

	Vector4D color = { 1.0f,1.0f,1.0f,1.0f };

	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	//	ビューポートシザー矩形
	ViewPort viewPort;

	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	ComPtr<ID3D12Resource> depthBuff;
public:
	PostEffect() {};
	~PostEffect() {};
	void Initialize(int width, int height, float weight, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	void Setting();
	void DrawLuminnce();
	void Draw(bool xBlur, bool yBlur, bool shadow, int handle1 = -1);
	void SetColor(const Vector4D& color_);

	ID3D12Resource* GetTextureBuff(int index = 0) { return texture[index].GetResourceBuff(); }
	ID3D12Resource** GetTextureBuffPtr(int index = 0) { return texture[index].GetResourceBuffAddress(); }
	int GetTextureNum() { return texNum; }
	D3D12_RESOURCE_BARRIER& GetResouceBarrier() { return barrierDesc; }
	ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap.Get(); }
	ID3D12DescriptorHeap* GetDSVHeap() { return dsvHeap.Get(); }
	Texture GetTexture() { return texture[0]; }
private:
	void SetVertices() override;
};

