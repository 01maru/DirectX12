#include "Object3D.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "PipelineManager.h"
#include "TextureManager.h"
#include "SceneManager.h"

#include "ConstBuffStruct.h"

Light* Object3D::light = nullptr;
GPipeline* Object3D::pipeline = nullptr;
ICamera* Object3D::camera = nullptr;
MyDirectX* Object3D::dx = MyDirectX::GetInstance();

void Object3D::SetLight(Light* light_)
{
	Object3D::light = light_;
}

void Object3D::SetPipeline(GPipeline* pipeline_)
{
	Object3D::pipeline = pipeline_;
}

void Object3D::SetCamera(ICamera* camera_)
{
	Object3D::camera = camera_;
}

void Object3D::SetModel(IModel* model_)
{
	model = model_;
}

void Object3D::SetCollider(BaseCollider* collider_)
{
	collider_->SetObject3D(this);
	//this->collider = collider_;
	CollisionManager::GetInstance()->AddCollider(collider_);
	MatUpdate();
	collider_->Update();
}

void Object3D::SetAttribute(unsigned short /*attribute*/)
{
	//collider->SetAttribute(attribute);
}

Object3D::~Object3D()
{
	//if (collider) {
	//	CollisionManager::GetInstance()->RemoveCollider(collider);
	//	delete collider;
	//}
}

Object3D* Object3D::Create(IModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Object3D* obj = new Object3D();
	if (obj == nullptr) {
		return nullptr;
	}

	// 初期化
	obj->Initialize();

	if (model) {
		obj->SetModel(model);
	}

	return obj;
}

void Object3D::Initialize()
{
	HRESULT result;

#pragma region ConstBuff

	transform.Initialize(sizeof(CBuff::CBuffObj3DTransform));
	//	定数バッファのマッピング
	result = transform.GetResource()->Map(0, nullptr, (void**)&cTransformMap);	//	マッピング
	assert(SUCCEEDED(result));

	shadowTransform.Initialize(sizeof(CBuff::CBuffObj3DTransform));
	//	定数バッファのマッピング
	result = shadowTransform.GetResource()->Map(0, nullptr, (void**)&cShadowTransMap);	//	マッピング
	assert(SUCCEEDED(result));

	lightMaterial.Initialize(sizeof(CBuff::CBuffLightMaterial));
	//	定数バッファのマッピング
	result = lightMaterial.GetResource()->Map(0, nullptr, (void**)&cLightMap);	//	マッピング
	assert(SUCCEEDED(result));

	skinData.Initialize(sizeof(CBuff::CBuffSkinData));
	//	定数バッファのマッピング
	result = skinData.GetResource()->Map(0, nullptr, (void**)&cSkinMap);	//	マッピング
	assert(SUCCEEDED(result));

	colorMaterial.Initialize(sizeof(CBuff::CBuffColorMaterial));
	//	定数バッファのマッピング
	result = colorMaterial.GetResource()->Map(0, nullptr, (void**)&cColorMap);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

	mat.Initialize();

	//	ボーンの初期化
	for (UINT i = 0; i < CBuff::MAX_BONES; i++)
	{
		cSkinMap->bones[i] = Matrix();
	}
}

void Object3D::ColliderUpdate()
{
	//if (collider) {
	//	collider->Update();
	//}
}

void Object3D::MatUpdate(ICamera* camera_)
{

#pragma region WorldMatrix
	mat.Update();
#pragma endregion

	//if (isBillboard) {
	//	const XMMATRIX& matBillboard = camera->GetBillboardMatrix();

	//	matWorld = XMMatrixIdentity();
	//	matWorld *= matScale; // ワールド行列にスケーリングを反映
	//	matWorld *= matRot; // ワールド行列に回転を反映
	//	matWorld *= matBillboard;
	//	matWorld *= matTrans; // ワールド行列に平行移動を反映
	//}

	// 親オブジェクトがあれば
	if (parent != nullptr) {
		mat.matWorld *= parent->mat.matWorld;
	}

	ICamera* cam = nullptr;
	if (camera_ != nullptr) {
		cam = camera_;
	}
	else {
		cam = camera;
	}
	const Matrix& matViewProjection = cam->GetViewProj();
	const Vector3D& cameraPos = cam->GetEye();

	cTransformMap->matViewProj = matViewProjection;
	cTransformMap->matViewProj = matViewProjection;
	if (model != nullptr) {
		cTransformMap->matWorld = model->GetModelTransform();
		cTransformMap->matWorld *= mat.matWorld;
	}
	else {
		cTransformMap->matWorld = mat.matWorld;
	}
	cTransformMap->cameraPos = cameraPos;

	const Matrix& matView_ = light->GetDirLightCamera(0)->GetViewProj();

	
	cShadowTransMap->matViewProj = matView_;
	if (model != nullptr) {
		cShadowTransMap->matWorld = model->GetModelTransform();
		cShadowTransMap->matWorld *= mat.matWorld;
	}
	else {
		cShadowTransMap->matWorld = mat.matWorld;
	}

	cLightMap->mLVP = matView_;
	cLightMap->cameraPos = light->GetDirLightCamera(0)->GetEye();

	cColorMap->color = color;
}

void Object3D::PlayAnimation()
{
	std::vector<Matrix> Transforms;

	animationTimer += 0.01f;
	model->BoneTransform(animationTimer, Transforms);

	for (UINT i = 0; i < model->GetNumBones(); i++)
	{
		cSkinMap->bones[i] = Transforms[i];
	}
}

void Object3D::Draw()
{
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	transform.SetGraphicsRootCBuffView(2);
	skinData.SetGraphicsRootCBuffView(4);
	colorMaterial.SetGraphicsRootCBuffView(5);

	light->SetGraphicsRootCBuffView(3);

	model->Draw();
}

void Object3D::DrawSilhouette()
{
	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("ModelSilhouette");
	pipeline_->Setting();
	pipeline_->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	transform.SetGraphicsRootCBuffView(2);
	skinData.SetGraphicsRootCBuffView(4);
	colorMaterial.SetGraphicsRootCBuffView(5);
	light->SetGraphicsRootCBuffView(3);

	model->Draw();
}

void Object3D::DrawShadow()
{
	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("Shadow");
	pipeline_->Setting();
	pipeline_->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shadowTransform.SetGraphicsRootCBuffView(2);
	lightMaterial.SetGraphicsRootCBuffView(3);

	model->Draw();
}

void Object3D::DrawShadowReciever()
{
	GPipeline* pipeline_= PipelineManager::GetInstance()->GetPipeline("ShadowReciever");
	pipeline_->Setting();
	pipeline_->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Texture shadowmap = SceneManager::GetInstance()->GetShadowMap();
	dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(shadowmap.GetHandle()));

	transform.SetGraphicsRootCBuffView(2);
	lightMaterial.SetGraphicsRootCBuffView(3);

	model->DrawShadowReciever();
}
