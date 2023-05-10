#include "GameScene.h"
#include "MyDebugCamera.h"
#include "GameCamera.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "MeshCollider.h"
#include "SceneManager.h"
#include "MyXAudio.h"
#include "NormalCamera.h"
#include "TextureManager.h"
#include "FbxModel.h"
#include "ObjModel.h"
#include "PipelineManager.h"
#include "ParticleManager.h"
#include "DebugTextManager.h"
#include "SceneManager.h"

void GameScene::CollisionUpdate()
{
}

void GameScene::MatUpdate()
{
	ground->MatUpdate();
	skydome->MatUpdate();
	player->MatUpdate();
	sphere->MatUpdate();
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete camera;
}

void GameScene::Initialize()
{
	collisionMan = CollisionManager::GetInstance();

	Light::GetInstance()->SetDirLightDir(0, Vector3D(0.0f, 1.0f, 0.0f));

	camera = new MyDebugCamera();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	LoadResources();

	Object3D::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Model", GPipeline::ALPHA_BLEND));
	Object3D::SetCamera(camera);

	Grass::SetPipeline(PipelineManager::GetInstance()->GetPipeline("GrassParticle", GPipeline::ALPHA_BLEND));
	Grass::SetCamera(camera);
	
	Particle::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Particle", GPipeline::ADD_BLEND));
	Particle::SetCamera(camera);

	Object2D::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Obj2D", GPipeline::ADD_BLEND));
	Object2D::SetCamera(camera);
	
	Player::SetCamera(camera);
}

void GameScene::Finalize()
{
	MyXAudio::GetInstance()->StopAllLoopSound();
}

void GameScene::LoadResources()
{
#pragma region Model

	modelSkydome = std::make_unique<ObjModel>("skydome");
	modelGround = std::make_unique<ObjModel>("ground");
	modelSphere = std::make_unique<ObjModel>("sphere", true);
	modelSword = std::make_unique<ObjModel>("chr_sword");

#pragma endregion
	//	天球
	skydome.reset(Object3D::Create(modelSkydome.get()));
	//	地面
	ground.reset(Object3D::Create(modelGround.get()));
	//	球
	sphere.reset(Object3D::Create(modelSphere.get()));
	sphere->SetPosition({ 0.0f,3.0f,0.0f });
	sphere->SetColor({ 1.0f,0.3f,0.3f });
	//	player
	player.reset(Object3D::Create(modelSword.get()));

#pragma region Sprite
	
#pragma endregion
}

void GameScene::Update()
{
#pragma region 更新処理

	if (input->GetTrigger(DIK_B)) {
		SceneManager::GetInstance()->SetNextScene("GAMESCENE");
	}

	camera->Update();

	//DebugTextManager::GetInstance()->Print("test", { 0,Window::window_height/2.0f }, 5);
#pragma endregion
	MatUpdate();
	CollisionUpdate();
	sphere->SetToon(input->GetKey(DIK_1));
	sphere->SetRimLight(input->GetKey(DIK_2));
}

void GameScene::DrawShadow()
{

}

void GameScene::Draw()
{
	//	天球
	skydome->DrawShadowReciever();
	//	地面
	ground->DrawShadowReciever();

	//player->Draw();

	sphere->Draw();

	//DebugTextManager::GetInstance()->Draw();
}
