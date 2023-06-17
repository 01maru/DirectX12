#pragma once
#include "Sprite.h"

class DebugTextManager
{
private:
	static Texture sFontTex;		// デバッグテキスト用のテクスチャ
	static size_t sMaxSpritesNum;	// 最大文字数
	static size_t sFontWidth;		// fontTex内1文字分の横幅
	static size_t sFontHeight;		// fontTex内1文字分の縦幅
	static size_t sFontLineCharNum;	// fontTex内1行分の文字数

	std::vector<Sprite> sprites;

	size_t index = 0;

	DebugTextManager() {};
	~DebugTextManager() {};
public:
	static DebugTextManager* GetInstance();
	DebugTextManager(const DebugTextManager& obj) = delete;
	DebugTextManager& operator=(const DebugTextManager& obj) = delete;

	void Initialize();
	void Print(const std::string& text, const Vector2D& pos, float size);
	void Draw();
};

