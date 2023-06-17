#include "DebugTextManager.h"
#include "TextureManager.h"

Texture DebugTextManager::sFontTex;
size_t DebugTextManager::sMaxSpritesNum = 256;
size_t DebugTextManager::sFontWidth = 9;
size_t DebugTextManager::sFontHeight = 18;
size_t DebugTextManager::sFontLineCharNum = 14;

DebugTextManager* DebugTextManager::GetInstance()
{
	static DebugTextManager instance;
	return &instance;
}

void DebugTextManager::Initialize()
{
	sFontTex = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/debugfont.png");
	sprites.resize(sMaxSpritesNum);
	for (size_t i = 0; i < sprites.size(); i++)
	{
		sprites[i].Initialize(sFontTex);
	}
}

void DebugTextManager::Print(const std::string& text, const Vector2D& pos, float size)
{
	for (size_t i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (index >= sMaxSpritesNum) {
			break;
		}

		const unsigned char& character = text[i];

		size_t fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		size_t fontIndexY = fontIndex / sFontLineCharNum;
		size_t fontIndexX = fontIndex % sFontLineCharNum;

		// 座標計算
		sprites[index].SetPosition({ pos.x + sFontWidth * size * i, pos.y });
		sprites[index].SetTextureLeftTop({ (float)fontIndexX * sFontWidth, (float)fontIndexY * sFontHeight });
		sprites[index].SetTextureSize({ (float)sFontWidth, (float)sFontHeight });
		sprites[index].SetSize({ sFontWidth * size, sFontHeight * size });
		sprites[index].Update();
		//sprites[index].SetTextureRect();
		// 文字を１つ進める
		index++;
	}
}

void DebugTextManager::Draw()
{
	for (size_t i = 0; i < sprites.size(); i++)
	{
		sprites[i].Draw();
	}

	index = 0;
}
