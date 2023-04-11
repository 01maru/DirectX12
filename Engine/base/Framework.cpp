#include "Framework.h"
#include "ParticleManager.h"
#include "DebugTextManager.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "ParticleManager.h"
#include "MyXAudio.h"

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
	//	�V���O���g���N���X��delete
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

	//	60fps�Œ�p
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
