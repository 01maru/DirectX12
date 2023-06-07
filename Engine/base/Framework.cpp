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
	//	������
	Initialize();

	//	�Q�[�����[�v
	while (gameroopFlag)
	{
		//	�X�V
		Update();

		//	�`��
		Draw();
	}

	Finalize();
}

void Framework::Initialize()
{
	//	winApi������
	Window::GetInstance()->Initialize();

	TextureManager::GetInstance()->Initialize();

	MyDirectX::GetInstance()->Initialize();

	PipelineManager::GetInstance()->Initialize();

	//	���[�h���s�����ۂ̔��F�e�N�X�`���̃��[�h
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

	//	60fps�Œ�p
	MyDirectX::GetInstance()->UpdateFPS();
}

void Framework::Finalize()
{
}
