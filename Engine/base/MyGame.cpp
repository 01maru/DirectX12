#include "MyGame.h"
#include "SceneManager.h"
#include "Light.h"
#include "Object3D.h"

void MyGame::Initialize()
{
	Framework::Initialize();

	Light* light = Light::GetInstance();
	//	ƒ‰ƒCƒg‚Ì‰Šú‰»
	light->Initialize();

#pragma region SetLight
	light->SetDirLightActive(0, true);
	light->SetDirLightShadow(0, true);

	Object3D::SetLight(light);
#pragma endregion

	SceneManager::GetInstance()->Initialize();
}

void MyGame::Update()
{
	Framework::Update();

	SceneManager::GetInstance()->Update();
	Light::GetInstance()->Update();
}

void MyGame::Draw()
{
	SceneManager::GetInstance()->Draw();
}

void MyGame::Finalize()
{
	Light::DeleteInstance();
	SceneManager::DeleteInstance();
	Framework::Finalize();
}
