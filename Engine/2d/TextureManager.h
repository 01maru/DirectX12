#pragma once
#include "Texture.h"
#include <vector>

class TextureManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static Texture whiteTexHandle;

	int textureNum = 0;
	std::vector<ComPtr<ID3D12Resource>> texBuff;
	std::vector<ComPtr<ID3D12Resource>> uploadBuff;
	std::vector<bool> texExist;
public:
	void SetWhiteTexHandle();		//	読み込み失敗用白色画像のセット
	static TextureManager* GetInstance();
	static void DeleteInstance();
	TextureManager();
	~TextureManager() {};
	void Initialize();
	void DeleteTexture(int handle);
	Texture LoadTextureGraph(const wchar_t* textureName);
	void CreateNoneGraphTexture(const std::string& texName, Texture& texture);
	//	Getter
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(int handle);
	const Texture& GetWhiteTexture() { return whiteTexHandle; }
};

