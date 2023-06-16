#include "TitleScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "NormalCamera.h"
#include "TextureManager.h"
#include "XAudioManager.h"

void TitleScene::LoadResources()
{
#pragma region Sound
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("titleBGM.wav");
	xAudioMan->LoadSoundWave("cursorMove.wav");
	xAudioMan->LoadSoundWave("decision.wav");
#pragma endregion

#pragma region Texture
	titleG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/title.png");
	pressG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/press.png");
#pragma endregion
}

void TitleScene::Initialize()
{
	camera = std::make_unique<NormalCamera>();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	LoadResources();

#pragma region Sprite
	backSprite_ = std::make_unique<Sprite>();
	backSprite_->Initialize(TextureManager::GetInstance()->GetWhiteTexture());
	backSprite_->SetPosition(Vector2D{ Window::sWIN_WIDTH / 2.0f,Window::sWIN_HEIGHT / 2.0f });
	backSprite_->SetSize(Vector2D{ Window::sWIN_WIDTH,Window::sWIN_HEIGHT });
	backSprite_->SetAnchorPoint(Vector2D{ 0.5f,0.5f });
	backSprite_->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	titleSprite_ = std::make_unique<Sprite>();
	titleSprite_->Initialize(titleG);
	titleSprite_->SetPosition(Vector2D{ Window::sWIN_WIDTH / 2.0f,200.0f });
	titleSprite_->SetAnchorPoint(Vector2D{ 0.5f,0.5f });

	pressSprite_ = std::make_unique<Sprite>();
	pressSprite_->Initialize(pressG);
	pressSprite_->SetPosition(Vector2D{ Window::sWIN_WIDTH / 2.0f,620.0f });
	pressSprite_->SetAnchorPoint(Vector2D{ 0.5f,0.5f });
#pragma endregion

	//	playBGM
	XAudioManager::GetInstance()->PlaySoundWave("titleBGM.wav", XAudioManager::BGM, true);
}

void TitleScene::Finalize()
{
	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();
}

void TitleScene::MatUpdate()
{
	//	背景
	backSprite_->Update();
	//	タイトル
	titleSprite_->Update();
	//	pressKey
	pressSprite_->Update();
}

void TitleScene::Update()
{
	if (InputManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_B)) {
		SceneManager::GetInstance()->SetNextScene("GAMESCENE");
	}

	MatUpdate();
}

void TitleScene::ImguiUpdate()
{
}

void TitleScene::DrawShadow() {}

void TitleScene::Draw()
{
	//	背景
	backSprite_->Draw();
	////	タイトル
	//titleSprite_->Draw();
	////	pressKey
	//pressSprite_->Draw();
}
