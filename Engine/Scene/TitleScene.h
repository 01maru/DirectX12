#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "MyXAudio.h"
#include "ICamera.h"
#include "Texture.h"

class TitleScene :public IScene
{
private:
	static MyXAudio* xAudio;

	int timer = 0;

	std::unique_ptr<ICamera> camera;
#pragma region Sprite
	std::unique_ptr<Sprite> pressSprite;
#pragma endregion

#pragma region sound
	int selectSound = -1;
	int bgmSound = -1;
#pragma endregion

#pragma region Texture
	Texture pressG;
#pragma endregion
public:
	TitleScene();
	~TitleScene() override;
	void Initialize() override;
	void Finalize() override;
	void LoadResources() override;
	void Update() override;
	void DrawShadow() override;
	void Draw() override;
private:
	void MatUpdate() override;
};

