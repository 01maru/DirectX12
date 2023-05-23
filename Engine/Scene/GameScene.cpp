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
	//player->CollisionUpdate();
	collisionMan->CheckAllCollisions();
}

void GameScene::MatUpdate()
{
	objData->MatUpdate();

	ParticleManager::GetInstance()->MatUpdate();
	ground->MatUpdate();
	hill->MatUpdate();
	skydome->MatUpdate();

	tree->MatUpdate();
	//tree->PlayAnimation();
	for (int i = 0; i < tree2.size(); i++)
	{
		tree2[i]->MatUpdate();
	}
	//sphere2->MatUpdate();

	for (int i = 0; i < grass.size(); i++)
	{
		grass[i].MatUpdate();
	}

	for (int i = 0; i < testVolLight.size(); i++)
	{
		testVolLight[i].MatUpdate();
	}
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//delete camera;
	for (int i = 0; i < tree2.size(); i++)
	{
		delete tree2[i];
	}
}

void GameScene::Initialize()
{
	collisionMan = CollisionManager::GetInstance();

	camera = new MyDebugCamera();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	//camera = Light::GetInstance()->GetDirLightCamera(0);

	Object3D::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Model", GPipeline::ALPHA_BLEND));
	Object3D::SetCamera(camera);
	LoadResources();

	Grass::SetPipeline(PipelineManager::GetInstance()->GetPipeline("GrassParticle", GPipeline::ALPHA_BLEND));
	Grass::SetCamera(camera);
	
	Particle::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Particle", GPipeline::ADD_BLEND));
	Particle::SetCamera(camera);

	Object2D::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Obj2D", GPipeline::ADD_BLEND));
	Object2D::SetCamera(camera);
	
	Player::SetCamera(camera);
	player = std::make_unique<Player>();
	//player->PlayerInitialize(modelSword.get());
	//float size = 0.05f;
	//player->SetScale({ size,size,size });
	//player->SetRotation({ 0.0f,MyMath::PI,0.0f });

	testVolLight.push_back(VolumeLightObj(Vector2D(5.0f, 25.0f), Vector3D(0.0f, 0.0f, -2.0f), 30.0f));
}

void GameScene::Finalize()
{
	MyXAudio::GetInstance()->StopAllLoopSound();
}

void GameScene::LoadResources()
{
	objData = std::make_unique<JSONLoader>();
	objData->LoadJSON("gamescene");

#pragma region Model
	modelSword = std::make_unique<FbxModel>("moveCube");

	modelSkydome = std::make_unique<ObjModel>("skydome");
	modelHill = std::make_unique<ObjModel>("ground1");
	modelGround = std::make_unique<ObjModel>("ground");
	//	丘
	modelTree = std::make_unique<ObjModel>("player");
	modelTree2 = std::make_unique<ObjModel>("tree2");
#pragma endregion
	//	天球
	skydome.reset(Object3D::Create(modelSkydome.get()));
	//skydome->SetPosition({ 0.0f,10.0f,0.0f });
	//	地面
	ground.reset(TouchableObject::Create(modelGround.get()));
	//ground->SetScale(Vector3D(1.0f, 1.0f, 0.1f));
	hill.reset(Object3D::Create(modelTree.get()));
	//float groundscale = 10.0f;
	//hill->SetScale(Vector3D(groundscale, 1.0f, 1.0f));
	//hill->SetPosition(Vector3D(0.0f, 0.0f, 10.0f));
	hill->SetScale(Vector3D(5.0f, 0.2f, 5.0f));
	hill->SetPosition(Vector3D(0.0f, 1.0f, 0.0f));

	tree.reset(Object3D::Create(modelTree.get()));
	//float size = 0.01f;
	tree->SetPosition({ 0.0f,3.0f,0.0f});
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	tree2.push_back(Object3D::Create(modelTree2.get()));
	for (int i = 0; i < tree2.size(); i++)
	{
		tree2[i]->SetPosition(Vector3D(-12.0f + 2.0f * i, 0.0f, 5.0f+MyMath::GetRand(0.0f,6.0f)));
	}
#pragma region Texture
	VolumeLightObj::SetLightGraph(TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/lightTex.jpg"));
	reimuG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/reimu.png");
	grassG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/grass.png");
#pragma endregion

#pragma region Sprite
	sprite = std::make_unique<Sprite>(SceneManager::GetInstance()->GetShadowMap());
	sprite->SetSize(Vector2D(200.0f, 200.0f));
	//sprite->SetTextureLeftTop(Vector2D(sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f));
	//sprite->SetTextureSize(Vector2D(sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f));
#pragma endregion
}

void GameScene::Update()
{
#pragma region 更新処理

	//	カメラ切り替え
	if (input->GetTrigger(DIK_V)) {
		isDebug = !isDebug;
		if (isDebug) {
			//delete camera;
			camera = new MyDebugCamera();
			camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
			Player::SetCamera(camera);
			Grass::SetCamera(camera);
			Object3D::SetCamera(camera);
		}
		else {
			delete camera;
			camera = Light::GetInstance()->GetDirLightCamera(0);
			Player::SetCamera(camera);
			Grass::SetCamera(camera);
			Object3D::SetCamera(camera);
		}
	}

	if (input->GetTrigger(DIK_B)) {
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}
	if (input->GetTrigger(DIK_O)) {
		ParticleManager::GetInstance()->AddMoveParticle(Vector3D(), Vector3D(0.0f, 0.2f, 0.0f), 60, 10);
	}

	camera->Update();

	sprite->Update();
	ParticleManager::GetInstance()->Update();

	//DebugTextManager::GetInstance()->Print("test", { 0,Window::window_height/2.0f }, 5);
#pragma endregion
	MatUpdate();
	CollisionUpdate();

	int dikkey = input->GetKey(DIK_UP) - input->GetKey(DIK_DOWN);
	lightColor += dikkey * 0.01f;
	if (lightColor < 0.0f) lightColor = 0.0f;
	Light::GetInstance()->SetDirLightColor(0, Vector3D(lightColor, lightColor, lightColor));
}

void GameScene::DrawShadow()
{
	tree->DrawShadow();
	//hill->DrawShadow();
}

void GameScene::Draw()
{
	//hill->Draw();
	//	木
	//tree->Draw();
	////hill->DrawShadowReciever();
	////	天球
	//skydome->Draw();
	////	地面
	//ground->Draw();

	objData->Draw();

	//player->Draw();

	//for (int i = 0; i < tree2.size(); i++)
	//{
	//	tree2[i]->Draw();
	//}
	//
	//for (int i = 0; i < grass.size(); i++)
	//{
	//	grass[i].Draw(grassG.GetHandle());
	//}
	//for (int i = 0; i < testVolLight.size(); i++)
	//{
	//	testVolLight[i].Draw();
	//}

	//sprite->Draw();

	//DebugTextManager::GetInstance()->Draw();
	//ParticleManager::GetInstance()->Draw();
}
