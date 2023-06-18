#include "Texture.h"
#include "TextureManager.h"

void Texture::Initialize(const std::string& texName, int32_t handle, ID3D12Resource* texBuff_ptr)
{
	name_ = texName;
	handle_ = handle;
	texBuff_ = texBuff_ptr;
}

void Texture::CreateNoTexture(const std::string& texName)
{
	TextureManager::GetInstance()->CreateNoneGraphTexture(texName, *this);
}
