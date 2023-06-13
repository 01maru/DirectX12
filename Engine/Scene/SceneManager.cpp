#include "SceneManager.h"
#include "Easing.h"
#include "ImGuiManager.h"
#include "Window.h"
#include "DirectX.h"

#include "LoadingSprite.h"
#include "SceneFactory.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::FirstSceneInitialize()
{
	scene_->Initialize();
	//	�摜�]��
	MyDirectX::GetInstance()->UploadTexture();
}

void SceneManager::SplashScreenInitialize()
{
	//	�񓯊�
	sceneInitInfo_ = std::async(std::launch::async, [this] {return FirstSceneInitialize(); });
	isSplashScreen_ = true;

	splashSprite_ = std::make_unique<SplashSprite>();
	splashSprite_->Initialize();

	sceneChangeCounter_.SetIsIncrement(true);
	sceneChangeCounter_.StartCount();

	//	�摜�]��
	MyDirectX::GetInstance()->UploadTexture();
}

void SceneManager::Initialize()
{
	sceneChangeCounter_.Initialize(60, true, true);

	sceneFactry_ = std::make_unique<SceneFactory>();
	scene_.reset(sceneFactry_->CreateScene("TITLESCENE"));

#pragma region Loading

	endLoading_ = true;
	loadObj_ = std::make_unique<LoadingSprite>();
	loadObj_->Initialize();
	loadObj_->SetIsLoading(!endLoading_);

#pragma endregion

#pragma region PostEffect

	mainScene = std::make_unique<PostEffect>();
	mainScene->Initialize(Window::window_width, Window::window_height, 1.0f, DXGI_FORMAT_R11G11B10_FLOAT);
	luminnce = std::make_unique<PostEffect>();
	luminnce->Initialize(Window::window_width, Window::window_height, 5.0f, DXGI_FORMAT_R11G11B10_FLOAT);
	xbulrluminnce = std::make_unique<PostEffect>();
	xbulrluminnce->Initialize(Window::window_width / 2, Window::window_height, 5.0f, DXGI_FORMAT_R11G11B10_FLOAT);
	ybulrluminnce = std::make_unique<PostEffect>();
	ybulrluminnce->Initialize(Window::window_width / 2, Window::window_height / 2, 5.0f, DXGI_FORMAT_R11G11B10_FLOAT);

	shadowEffect = std::make_unique<PostEffect>();
	shadowEffect->Initialize(Window::window_width, Window::window_height, 1.0f, DXGI_FORMAT_R32G32_FLOAT);
	
	xbulr = std::make_unique<PostEffect>();
	xbulr->Initialize(Window::window_width / 2, Window::window_height, 1.0f, DXGI_FORMAT_R32G32_FLOAT);
	ybulr = std::make_unique<PostEffect>();
	ybulr->Initialize(Window::window_width / 2, Window::window_height / 2, 1.0f, DXGI_FORMAT_R32G32_FLOAT);

#pragma endregion

#ifdef _DEBUG
	ImGuiManager::GetInstance()->Initialize();
#endif // _DEBUG

#pragma region SplashScreen

	//	release��������X�v���b�V���X�N���[������
#ifdef NDEBUG
	SplashScreenInitialize();
#endif // NDEBUG


	//	debug��������X�v���b�V���X�N���[���Ȃ�
#ifdef _DEBUG
	FirstSceneInitialize();
#endif // _DEBUG

#pragma endregion
}

void SceneManager::Finalize()
{
	scene_->Finalize();

#ifdef _DEBUG

	ImGuiManager::GetInstance()->Finalize();

#endif // _DEBUG
}

void SceneManager::ScreenColorUpdate()
{
	sceneChangeCounter_.Update();

	if (sceneChangeCounter_.GetIsActive()) {
		float color = Easing::lerp(1.0f, 0.0f, sceneChangeCounter_.GetCountPerMaxCount());
		screenColor_.x = color;
		screenColor_.y = color;
		screenColor_.z = color;

		//	�F�ݒ�
		mainScene->SetColor(screenColor_);
	}
}

void SceneManager::SplashUpdate()
{
#ifdef NDEBUG

	//	�X�v���b�V���X�N���[������Ȃ�������
	if (!isSplashScreen_) return;

	if (sceneChangeCounter_.GetFrameCount() == sceneChangeCounter_.GetMaxFrameCount()) splashSprite_->StartCounter();
	
	splashSprite_->Update();

	std::future_status loadStatus = sceneInitInfo_.wait_for(std::chrono::seconds(0));
	if (loadStatus == std::future_status::ready && splashSprite_->SplashEnd()) {

		//	�X�v���b�V���X�N���[���I���
		isSplashScreen_ = false;
		scene_->Update();

		sceneChangeCounter_.SetIsIncrement(false);
		sceneChangeCounter_.StartCount();

		screenColor_ = { 0.0f,0.0f,0.0f,1.0f };
		//	�F�ݒ�
		mainScene->SetColor(screenColor_);
	}

#endif // NDEBUG
}

void SceneManager::SceneFadeInUpdate()
{
	if (!endLoading_)		return;
	if (!sceneInitialized_)	return;

	//	�t�F�[�h�C���ς�
	bool fadedIn = sceneChangeCounter_.GetFrameCount() == 0;
	if (fadedIn) {
		//	��������
		scene_->Update();
	}
}

void SceneManager::SceneFadeOutUpdate()
{
	if (!endLoading_)		return;
	if (sceneInitialized_)	return;

	//	�t�F�[�h�A�E�g�ς�
	bool fadedOut = sceneChangeCounter_.GetFrameCount() == sceneChangeCounter_.GetMaxFrameCount();
	if (fadedOut) {
		//	�t�F�[�h�A�E�g�ς݃��[�h��ʂ�
		endLoading_ = false;
		//	�񓯊�
		sceneInitInfo_ = std::async(std::launch::async, [this] {return SceneChange(); });
	}
}

void SceneManager::SceneAsyncUpdate()
{
	//	���[�h������Ȃ�������
	if (endLoading_) return;

	std::future_status loadStatus = sceneInitInfo_.wait_for(std::chrono::seconds(0));
	if (loadStatus == std::future_status::ready) {
		//	���[�h�I���t���O
		endLoading_ = true;
		loadObj_->SetIsLoading(!endLoading_);
		//	�t�F�[�h�C��
		sceneChangeCounter_.SetIsIncrement(false);
		sceneChangeCounter_.StartCount();
	}
}

void SceneManager::SceneUpdate()
{
	//	�X�v���b�V���X�N���[����������
	if (isSplashScreen_) return;

	SceneFadeInUpdate();
	SceneFadeOutUpdate();
	
	SceneAsyncUpdate();
}

void SceneManager::ImguiUpdate()
{
#ifdef _DEBUG

	ImGuiManager::GetInstance()->Begin();
	if (endLoading_) {
		scene_->ImguiUpdate();
	}
	ImGuiManager::GetInstance()->End();

#endif // _DEBUG
}

void SceneManager::Update()
{
	ScreenColorUpdate();

	SplashUpdate();

	SceneUpdate();

	loadObj_->Update();

	ImguiUpdate();
}

void SceneManager::Draw()
{
	MyDirectX* dx = MyDirectX::GetInstance();

#pragma region DrawScreen
	FLOAT shadowClearColor_[] = { 1.0f,1.0f,1.0f,1.0f };

	dx->PrevPostEffect(shadowEffect.get(), shadowClearColor_);

	if (endLoading_ && !isSplashScreen_) {
		scene_->DrawShadow();
	}
	
	dx->PostEffectDraw(shadowEffect.get());

	dx->PrevPostEffect(xbulr.get(), shadowClearColor_);

	shadowEffect->Draw(true, false, true);

	dx->PostEffectDraw(xbulr.get());

	dx->PrevPostEffect(ybulr.get(), shadowClearColor_);

	xbulr->Draw(false, true, true);

	dx->PostEffectDraw(ybulr.get());
	
	dx->PrevPostEffect(mainScene.get());

	if (endLoading_ && !isSplashScreen_) {
		scene_->Draw();
	}

	dx->PostEffectDraw(mainScene.get());


	FLOAT luminnceClearColor_[] = { 0.0f,0.0f,0.0f,1.0f };
	dx->PrevPostEffect(luminnce.get(), luminnceClearColor_);

	mainScene->DrawLuminnce();

	dx->PostEffectDraw(luminnce.get());


	dx->PrevPostEffect(xbulrluminnce.get());

	luminnce->Draw(true, false, false);

	dx->PostEffectDraw(xbulrluminnce.get());

	dx->PrevPostEffect(ybulrluminnce.get());

	xbulrluminnce->Draw(false, true, false);

	dx->PostEffectDraw(ybulrluminnce.get());
#pragma endregion

#pragma region MultiPath
	FLOAT clearColor_[] = { 0.0f,0.0f,0.0f,1.0f };
	dx->PrevDraw(clearColor_);

	if (!isSplashScreen_) {
		mainScene->Draw(false, false, false, ybulrluminnce->GetTexture().GetHandle());
	}
	
	loadObj_->Draw();

#ifdef NDEBUG

	if (isSplashScreen_) {
		splashSprite_->Draw();
	}

#endif // NDEBUG

#ifdef _DEBUG
	ImGuiManager::GetInstance()->Draw();
#endif // _DEBUG
	dx->PostDraw();
#pragma endregion
}

void SceneManager::SceneChange()
{
	if (nextScene_ != nullptr) {
		if (scene_ != nullptr) {
			scene_->Finalize();
		}

		scene_.reset(nextScene_.get());
		scene_->Initialize();
		//	�摜�]��
		MyDirectX::GetInstance()->UploadTexture();
		sceneInitialized_ = true;
		nextScene_.release();
	}
}

void SceneManager::SetNextScene(const std::string& sceneName)
{
	nextScene_.reset(sceneFactry_->CreateScene(sceneName));
	
	//	nextScene���Z�b�g���ꂽ��
	if (nextScene_ != nullptr) {
		sceneInitialized_ = false;
		loadObj_->SetIsLoading(true);

		sceneChangeCounter_.SetIsIncrement(true);
		sceneChangeCounter_.StartCount();
	}
}
