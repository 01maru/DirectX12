#pragma once
#include "Sprite.h"

class DebugTextManager
{
private:
	static Texture fontTex;		// デバッグテキスト用のテクスチャ
	static int maxSpritesNum;	// 最大文字数
	static int fontWidth;		// fontTex内1文字分の横幅
	static int fontHeight;		// fontTex内1文字分の縦幅
	static int fontLineCharNum;	// fontTex内1行分の文字数

	std::vector<Sprite> sprites;

	int index = 0;

	DebugTextManager() {};
	~DebugTextManager() {};
public:
	static DebugTextManager* GetInstance();
	static void DeleteInstance();

	DebugTextManager(const DebugTextManager& obj) = delete;
	DebugTextManager& operator=(const DebugTextManager& obj) = delete;

	void Initialize();
	void Print(const std::string& text, const Vector2D& pos, float size);
	void Draw();
};

