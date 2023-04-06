#include "Framework.h"

void Framework::Run()
{
	Initialize();
	//	ƒQ[ƒ€ƒ‹[ƒv
	while (true)
	{
		Update();

		if (IsEndGameRoop()) { break; }

		Draw();
	}
	Finalize();
}

void Framework::Initialize()
{
	win = Window::GetInstance();
	win->Initialize();

	textureMan = TextureManager::GetInstance();
	textureMan->Initialize();

	dx = MyDirectX::GetInstance();
	dx->Initialize();

	pipelineMan = PipelineManager::GetInstance();
	pipelineMan->Initialize();

	textureMan->SetWhiteTexHandle();

	joypad = InputJoypad::GetInstance();
	input = Input::GetInstance();
}

void Framework::Update()
{
	isEndRoopFlag = win->MsgUpdate() || input->GetTrigger(DIK_ESCAPE);

	input->Update();
	joypad->Update();
	dx->UpdateFPS();
}

void Framework::Finalize()
{
	SpriteCommon::DeleteInstance();
	InputJoypad::DeleteInstance();
	Input::DeleteInstance();
	MyXAudio::DeleteInstance();
	PipelineManager::DeleteInstance();
	MyDirectX::DeleteInstance();
	TextureManager::DeleteInstance();
	Window::DeleteInstance();
}
