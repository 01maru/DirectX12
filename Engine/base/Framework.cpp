#include "Framework.h"
#include "ParticleManager.h"
#include "DebugTextManager.h"
#include "TextureManager.h"
#include "PipelineManager.h"

#include "InputManager.h"
#include "XAudioManager.h"
#include "FPSController.h"

void Framework::Run()
{
	//	初期化
	Initialize();

	//	ゲームループ
	while (gameroopFlag_)
	{
		//	更新
		Update();

		//	描画
		Draw();
	}

	Finalize();
}

void Framework::Initialize()
{
	//	winApi初期化
	Window::GetInstance()->Initialize();

	TextureManager::GetInstance()->Initialize();

	MyDirectX::GetInstance()->Initialize();

	PipelineManager::GetInstance()->Initialize();

	//	ロード失敗した際の白色テクスチャのロード
	TextureManager::GetInstance()->SetWhiteTexHandle();

	InputManager::GetInstance()->Initialize();

	ParticleManager::GetInstance()->Initialize();

	DebugTextManager::GetInstance()->Initialize();

	FPSController::GetInstance()->Initialize();

	XAudioManager::GetInstance()->Initialize();
}

void Framework::Update()
{
	gameroopFlag_ = !(Window::GetInstance()->MsgUpdate() || InputManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_ESCAPE));

	InputManager::GetInstance()->Update();

	//	60fps固定
	FPSController::GetInstance()->Update();
}

void Framework::Finalize()
{
	XAudioManager::GetInstance()->Finalize();
	Window::GetInstance()->Finalize();
}
