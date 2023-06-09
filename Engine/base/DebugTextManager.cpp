#include "DebugTextManager.h"
#include "TextureManager.h"

Texture DebugTextManager::fontTex;
int DebugTextManager::maxSpritesNum = 256;
int DebugTextManager::fontWidth = 9;
int DebugTextManager::fontHeight = 18;
int DebugTextManager::fontLineCharNum = 14;

DebugTextManager* DebugTextManager::GetInstance()
{
	static DebugTextManager instance;
	return &instance;
}

void DebugTextManager::Initialize()
{
	fontTex = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/debugfont.png");
	sprites.resize(maxSpritesNum);
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i].Initialize(fontTex);
	}
}

void DebugTextManager::Print(const std::string& text, const Vector2D& pos, float size)
{
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (index >= maxSpritesNum) {
			break;
		}

		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCharNum;
		int fontIndexX = fontIndex % fontLineCharNum;

		// 座標計算
		sprites[index].SetPosition({ pos.x + fontWidth * size * i, pos.y });
		sprites[index].SetTextureLeftTop({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight });
		sprites[index].SetTextureSize({ (float)fontWidth, (float)fontHeight });
		sprites[index].SetSize({ fontWidth * size, fontHeight * size });
		sprites[index].Update();
		sprites[index].SetTextureRect();
		// 文字を１つ進める
		index++;
	}
}

void DebugTextManager::Draw()
{
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i].Draw();
	}

	index = 0;
}
