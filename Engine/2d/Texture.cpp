#include "Texture.h"

void Texture::Create(const std::string& texName, int handle_, ID3D12Resource* texBuff_ptr)
{
	name = texName;
	handle = handle_;
	texBuff = texBuff_ptr;
}
