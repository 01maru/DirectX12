#include "TitleScene.h"
#include "Input.h"
#include "InputJoypad.h"
#include "SceneManager.h"
#include "NormalCamera.h"
#include "TextureManager.h"
#include "Easing.h"

void TitleScene::Initialize()
{
	camera = std::make_unique<NormalCamera>();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	LoadResources();
#pragma region Sprite
	//backSprite = std::make_unique<Sprite>(TextureManager::GetInstance()->GetWhiteTexture());
	//backSprite->SetPosition(Vector2D{ Window::window_width / 2.0f,Window::window_height / 2.0f });
	//backSprite->SetSize(Vector2D{ Window::window_width,Window::window_height });
	//backSprite->SetAnchorPoint(Vector2D{ 0.5f,0.5f });
	//backSprite->TransferVertex();
	//titleSprite = std::make_unique<Sprite>(titleG);
	//titleSprite->SetPosition(Vector2D{ Window::window_width / 2.0f,200.0f });
	//titleSprite->SetSize(titleSize);
	//titleSprite->SetAnchorPoint(Vector2D{ 0.5f,0.5f });
	//titleSprite->SetColor({ 1.0f,1.0f,1.0f,0.51f });
	//titleSprite->TransferVertex();
	//pressSprite = std::make_unique<Sprite>(pressG);
	//pressSprite->SetSize(pressSize);
	//pressSprite->SetPosition(Vector2D{ Window::window_width / 2.0f,620.0f });
	//pressSprite->SetAnchorPoint(Vector2D{ 0.5f,0.5f });
	//pressSprite->SetColor({ 1.0f,1.0f,1.0f,0.5f });
	//pressSprite->TransferVertex();
#pragma endregion
}

void TitleScene::Finalize()
{
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
	if (Input::GetInstance()->GetTrigger(DIK_B)) {
		SceneManager::GetInstance()->SetNextScene("GAMESCENE");
	}
	//int scalespd = 30;
	//if (endScene) {
	//	Vector2D size = pressSize;
	//	int t = timer;
	//	while (t > scalespd) t -= scalespd;
	//	size *= (1.0f + (float)Easing::EaseOut(0.0f, 0.1f, t / (float)scalespd, 4));
	//	pressSprite->SetSize(size);
	//	pressSprite->TransferVertex();

	//	if (timer >= 60) {
	//		SceneManager::GetInstance()->SetNextScene("GAMESCENE");
	//	}
	//}
	//else {
	//	Vector2D size = pressSize;
	//	size *= (1.0f + cosf(timer / (float)scalespd) * 0.1f);
	//	pressSprite->SetSize(size);
	//	pressSprite->TransferVertex();

	//	//	press
	//	if (Input::GetInstance()->GetTrigger(DIK_SPACE)) {
	//		endScene = true;
	//		timer = 0;
	//	}
	//}

	MatUpdate();
}

void TitleScene::ImguiUpdate()
{
}

void TitleScene::DrawShadow()
{

}

void TitleScene::Draw()
{
	//backSprite->Draw();
	//titleSprite->Draw();
	//pressSprite->Draw();
}

void TitleScene::MatUpdate()
{
	//titleSprite->SetTextureRect();
	//pressSprite->SetTextureRect();
	//backSprite->MatUpdate();
}
