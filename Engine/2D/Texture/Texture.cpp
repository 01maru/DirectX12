#include "Texture.h"
#include "TextureManager.h"

void Texture::Initialize(const std::string& texName, int handle_, ID3D12Resource* texBuff_ptr)
{
	name = texName;
	handle = handle_;
	texBuff = texBuff_ptr;
}

void Texture::CreateNoTexture(const std::string& texName)
{
	TextureManager::GetInstance()->CreateNoneGraphTexture(texName, *this);
}
