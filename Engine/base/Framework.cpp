#include "Framework.h"
#include "ParticleManager.h"
#include "DebugTextManager.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "ParticleManager.h"
#include "MyXAudio.h"
#include "Input.h"
#include "InputJoypad.h"

void Framework::Run()
{
	//	初期化
	Initialize();

	//	ゲームループ
	while (gameroopFlag)
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

	Input::GetInstance()->Initialize();

	ParticleManager::GetInstance()->Initialize();

	DebugTextManager::GetInstance()->Initialize();
}

void Framework::Update()
{
	gameroopFlag = !(Window::GetInstance()->MsgUpdate() || Input::GetInstance()->GetTrigger(DIK_ESCAPE));

	Input::GetInstance()->Update();
	InputJoypad::GetInstance()->Update();

	//	60fps固定用
	MyDirectX::GetInstance()->UpdateFPS();
}

void Framework::Finalize()
{
}
