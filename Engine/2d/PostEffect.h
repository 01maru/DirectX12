#pragma once
#include <d3d12.h>
#include "ViewPort.h"
#include "ScissorRect.h"
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

	std::vector<ScreenVertex> vertices;
	UINT indexSize = 6;
	unsigned short indices[6] = {};

	D3D12_RESOURCE_BARRIER barrierDesc;

	Vector4D color = { 1.0f,1.0f,1.0f,1.0f };

	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	//	ビューポート
	ViewPort viewPort;
	// シザー矩形
	ScissorRect scissorRect;
public:
	PostEffect() {};
	~PostEffect() {};
	void Initialize();

	void Setting();
	void Draw();
	void SetColor(const Vector4D& color_);

	ID3D12Resource* GetTextureBuff(int index = 0) { return texture[index].GetResourceBuff(); }
	ID3D12Resource** GetTextureBuffPtr(int index = 0) { return texture[index].GetResourceBuffAddress(); }
	int GetTextureNum() { return texNum; }
	D3D12_RESOURCE_BARRIER& GetResouceBarrier() { return barrierDesc; }
	ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap.Get(); }
	Texture GetTexture() { return texture[0]; }
private:
	void SetVertices() override;
};

