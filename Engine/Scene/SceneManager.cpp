#include "SceneManager.h"
#include "Easing.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Window.h"
#include "PipelineManager.h"

#include "LoadingSprite.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::FirstSceneInitialize()
{
	scene->Initialize();
	//	�摜�]��
	MyDirectX::GetInstance()->UploadTexture();
}

void SceneManager::Initialize()
{
	sceneFactry = std::make_unique<SceneFactory>();
	scene = sceneFactry->CreateScene("GAMESCENE");

#pragma region Loading

	endLoading = true;
	loadObj = std::make_unique<LoadingSprite>();
	loadObj->Initialize();
	loadObj->SetIsLoading(!endLoading);

#pragma endregion

#pragma region SplashScreen

	//	�񓯊�
	sceneInitInfo = std::async(std::launch::async, [this] {return FirstSceneInitialize(); });
	isSplashScreen = true;

	rogoUI = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/rogo.png");
	splashSprite = std::make_unique<Sprite>();
	splashSprite->Initialize(rogoUI);
	splashSprite->SetPosition(Vector2D{ Window::window_width / 2.0f,Window::window_height / 2.0f });
	splashSprite->SetAnchorPoint(Vector2D{ 0.5f,0.5f });
	//splashSprite->SetSize(Vector2D{ 500,64 });
	//splashSprite->TransferVertex();

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

	screenColor = { 0.0f,0.0f,0.0f,1.0f };
	//	�F�ݒ�
	mainScene->SetColor(screenColor);

#pragma endregion

	ImGuiManager::GetInstance()->Initialize();

	//	�摜�]��
	MyDirectX::GetInstance()->UploadTexture();
}

void SceneManager::Finalize()
{
	scene->Finalize();
	ImGuiManager::GetInstance()->Finalize();
}

void SceneManager::Update()
{
	static bool easeInSplashScreen = true;
	static int splashCount = 0;

	if (isSplashScreen) {
		float color = 0.0f;
		if (splashCount == 2) color = 1.0f;

		else if (splashCount == 0 || splashCount == 4) color = 0.0f;

		else color = Easing::lerp(0.0f, 1.0f, (float)sceneChangeTimer / S_SCENE_CHANGE_TIME);

		splashSprite->SetColor({ 1.0f,1.0f,1.0f,color });
		splashSprite->Update();

		std::future_status loadStatus = sceneInitInfo.wait_for(std::chrono::seconds(0));
		if (loadStatus == std::future_status::ready && !easeInSplashScreen) {
			if (sceneChangeTimer > 0) {
				sceneChangeTimer--;
			}
			else {
				splashCount++;
				sceneChangeTimer = S_SCENE_CHANGE_TIME;

				//	�X�v���b�V���X�N���[���I���t���O
				if (splashCount == 5) {
					isSplashScreen = false;
					scene->Update();
				}
			}
		}
		else {
			if (sceneChangeTimer < S_SCENE_CHANGE_TIME) {
				sceneChangeTimer++;
			}
			else {
				if (splashCount != 1) {
					sceneChangeTimer = 0;
				}
				else {
					easeInSplashScreen = false;
				}
				splashCount++;
			}
		}
	}
	else {
		if (endLoading) {
			if (sceneInitialized) {

				//	�t�F�[�h�C��
				if (sceneChangeTimer > 0) {
					sceneChangeTimer--;

					float color = Easing::lerp(1.0f, 0.0f, sceneChangeTimer / (float)S_SCENE_CHANGE_TIME);
					screenColor.x = color;
					screenColor.y = color;
					screenColor.z = color;

					//	�F�ݒ�
					mainScene->SetColor(screenColor);

					scene->MatUpdate();
				}
				else {
					//	��������
					scene->Update();
				}
			}
			else {
				//	nextScene���Z�b�g���ꂽ��
				loadObj->SetIsLoading(true);

				//	�t�F�[�h�A�E�g
				if (sceneChangeTimer < S_SCENE_CHANGE_TIME) {
					sceneChangeTimer++;

					float color = Easing::lerp(1.0f, 0.0f, sceneChangeTimer / (float)S_SCENE_CHANGE_TIME);
					screenColor.x = color;
					screenColor.y = color;
					screenColor.z = color;

					//	�F�ݒ�
					mainScene->SetColor(screenColor);
				}

				if (sceneChangeTimer >= S_SCENE_CHANGE_TIME) {
					//	�t�F�[�h�A�E�g�ς݃��[�h��ʂ�
					endLoading = false;
					//	�񓯊�
					sceneInitInfo = std::async(std::launch::async, [this] {return SceneChange(); });
					sceneChangeTimer = S_SCENE_CHANGE_TIME;
				}
			}
		}
		else {
			std::future_status loadStatus = sceneInitInfo.wait_for(std::chrono::seconds(0));
			if (loadStatus == std::future_status::ready) {
				//	���[�h�I���t���O
				endLoading = true;
				loadObj->SetIsLoading(!endLoading);
			}
		}
	}

	loadObj->Update();

#ifdef _DEBUG

	ImguiUpdate();

#endif // _DEBUG
}

void SceneManager::ImguiUpdate()
{
	ImGuiManager::GetInstance()->Begin();
	if (endLoading) {
		scene->ImguiUpdate();
	}
	ImGuiManager::GetInstance()->End();
}

void SceneManager::Draw()
{
	MyDirectX* dx = MyDirectX::GetInstance();

#pragma region DrawScreen
	dx->PrevPostEffect(shadowEffect.get());

	if (endLoading && !isSplashScreen) {
		scene->DrawShadow();
	}
	
	dx->PostEffectDraw(shadowEffect.get());

	dx->PrevPostEffect(xbulr.get());

	shadowEffect->Draw(true, false, true);

	dx->PostEffectDraw(xbulr.get());

	dx->PrevPostEffect(ybulr.get());

	xbulr->Draw(false, true, true);

	dx->PostEffectDraw(ybulr.get());
	
	dx->PrevPostEffect(mainScene.get());

	if (endLoading && !isSplashScreen) {
		scene->Draw();
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
	dx->PrevDraw();

	if (!isSplashScreen) {
		mainScene->Draw(false, false, false, ybulrluminnce->GetTexture().GetHandle());
	}
	
	loadObj->Draw();

	if (isSplashScreen) {
		splashSprite->Draw(PipelineManager::GetInstance()->GetPipeline("LoadingSprite"));
	}

	ImGuiManager::GetInstance()->Draw();
	dx->PostDraw();
#pragma endregion
}

void SceneManager::SceneChange()
{
	if (nextScene) {
		if (scene) {
			scene->Finalize();
			delete scene;
		}

		scene = nextScene;
		scene->Initialize();
		//	�摜�]��
		MyDirectX::GetInstance()->UploadTexture();
		sceneInitialized = true;
		nextScene = nullptr;
	}
}

void SceneManager::SetNextScene(const std::string& sceneName)
{
	nextScene = sceneFactry->CreateScene(sceneName);
	if (nextScene) sceneInitialized = false;
}
