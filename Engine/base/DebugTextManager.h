#pragma once
#include "Sprite.h"

class DebugTextManager
{
private:
	static Texture fontTex;		// �f�o�b�O�e�L�X�g�p�̃e�N�X�`��
	static int maxSpritesNum;	// �ő啶����
	static int fontWidth;		// fontTex��1�������̉���
	static int fontHeight;		// fontTex��1�������̏c��
	static int fontLineCharNum;	// fontTex��1�s���̕�����

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

