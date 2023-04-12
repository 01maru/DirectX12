#include "TitleScene.h"
#include "Input.h"
#include "InputJoypad.h"
#include "SceneManager.h"
#include "NormalCamera.h"
#include "TextureManager.h"
#include "Easing.h"

MyXAudio* TitleScene::xAudio = nullptr;
Vector2D TitleScene::titleSize = Vector2D{ 640,360 };
Vector2D TitleScene::pressSize = Vector2D{ 304,171 };

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	xAudio = MyXAudio::GetInstance();

	camera = std::make_unique<NormalCamera>();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	LoadResources();
#pragma region Sprite
	titleSprite = std::make_unique<Sprite>(titleG);
	titleSprite->SetPosition(Vector2D{ Window::window_width / 2.0f,200 });
	titleSprite->SetSize(titleSize);
	titleSprite->SetAnchorPoint(Vector2D{ 0.5,0.5 });
	titleSprite->TransferVertex();
	pressSprite = std::make_unique<Sprite>(pressG);
	pressSprite->SetSize(pressSize);
	pressSprite->SetPosition(Vector2D{ Window::window_width / 2.0f,620 });
	pressSprite->SetAnchorPoint(Vector2D{ 0.5,0.5 });
	pressSprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	pressSprite->TransferVertex();
#pragma endregion
}

void TitleScene::Finalize()
{
	xAudio->StopAllLoopSound();
}

void TitleScene::LoadResources()
{
#pragma region Sound

#pragma endregion

#pragma region Texture
	titleG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/title.png");
	pressG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/press.png");
#pragma endregion
}

void TitleScene::Update()
{
	timer++;
	int scalespd = 30;
	if (endScene) {
		Vector2D size = pressSize;
		int t = timer;
		while (t > scalespd) t -= scalespd;
		size *= (1.0f + (float)Easing::EaseOut(0.0f, 0.1f, t / (float)scalespd, 4));
		pressSprite->SetSize(size);
		pressSprite->TransferVertex();

		if (timer >= 60) {
			SceneManager::GetInstance()->SetNextScene("SELECTSCENE");
		}
	}
	else {
		Vector2D size = pressSize;
		size *= (1.0f + cosf(timer / (float)scalespd) * 0.1f);
		pressSprite->SetSize(size);
		pressSprite->TransferVertex();

		//	press
		if (Input::GetInstance()->GetTrigger(DIK_SPACE)) {
			endScene = true;
			timer = 0;
		}
	}

	MatUpdate();
}

void TitleScene::DrawShadow()
{

}

void TitleScene::Draw()
{
	titleSprite->Draw();
	pressSprite->Draw();
}

void TitleScene::MatUpdate()
{
	titleSprite->SetTextureRect();
	pressSprite->SetTextureRect();
}
