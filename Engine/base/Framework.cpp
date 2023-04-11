#include "Framework.h"
#include "ParticleManager.h"
#include "DebugTextManager.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "ParticleManager.h"
#include "MyXAudio.h"

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
	//	シングルトンクラスのdelete
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

	joypad = InputJoypad::GetInstance();
	input = Input::GetInstance();

	ParticleManager::GetInstance()->Initialize();

	DebugTextManager::GetInstance()->Initialize();
}

void Framework::Update()
{
	gameroopFlag = !(Window::GetInstance()->MsgUpdate() || input->GetTrigger(DIK_ESCAPE));

	input->Update();
	joypad->Update();

	//	60fps固定用
	MyDirectX::GetInstance()->UpdateFPS();
}

void Framework::Finalize()
{
	DebugTextManager::DeleteInstance();
	ParticleManager::DeleteInstance();
	SpriteCommon::DeleteInstance();
	InputJoypad::DeleteInstance();
	Input::DeleteInstance();
	MyXAudio::DeleteInstance();
	PipelineManager::DeleteInstance();
	MyDirectX::DeleteInstance();
	TextureManager::DeleteInstance();
	Window::DeleteInstance();
}
