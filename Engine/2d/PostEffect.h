#pragma once
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "Texture.h"

//	‚Ø‚çƒ|ƒŠƒSƒ“
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

	Vector4D color = { 1.0f,1.0f,1.0f,1.0f };
public:
	PostEffect() {};
	~PostEffect() {};
	static PostEffect* GetInstance();
	static void DeleteInstance();
	void Initialize();

	void Draw();
	void SetColor(const Vector4D& color_);

	ID3D12Resource* GetTextureBuff(int index = 0) { return texture[index].GetResourceBuff(); }
	ID3D12Resource** GetTextureBuffPtr(int index = 0) { return texture[index].GetResourceBuffPtrPtr(); }
	int GetTextureNum() { return texNum; }
private:
	void SetVertices() override;
};

