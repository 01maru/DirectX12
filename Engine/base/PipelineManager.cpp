#include "PipelineManager.h"
#include "Shader.h"

PipelineManager::~PipelineManager()
{
	modelPipeline.clear();
	obj2DPipeline.clear();
}

PipelineManager* PipelineManager::GetInstance()
{
	static PipelineManager* instance = new PipelineManager;
	return instance;
}

void PipelineManager::DeleteInstance()
{
	delete PipelineManager::GetInstance();
}

void PipelineManager::Initialize()
{
	int blendMordNum = 5;

#pragma region Model
	modelPipeline.reserve(blendMordNum);

	Shader objShader(L"Resources/shader/ObjVS.hlsl", L"Resources/shader/ObjPS.hlsl");

	D3D12_INPUT_ELEMENT_DESC modelInputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	法線ベクトル
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
		{"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};

	for (int i = 0; i < blendMordNum; i++)
	{
		//	pipeline追加
		modelPipeline.emplace_back(new GPipeline());
		
		GPipeline* modelpipeline_ = modelPipeline.back();
		modelpipeline_->Init(objShader, modelInputLayout, _countof(modelInputLayout), 4, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
		modelpipeline_->SetBlend(i);
	}
#pragma endregion

#pragma region Obj2D
	obj2DPipeline.reserve(blendMordNum);
	Shader test2dShader(L"Resources/shader/SpriteVS.hlsl", L"Resources/shader/SpritePS.hlsl");

	D3D12_INPUT_ELEMENT_DESC inputLayout2D[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};

	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* obj2Dpipe_ = new GPipeline();
		obj2Dpipe_->Init(test2dShader, inputLayout2D, _countof(inputLayout2D), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, D3D12_DEPTH_WRITE_MASK_ZERO);
		obj2Dpipe_->SetBlend(i);

		//	pipeline追加
		obj2DPipeline.emplace_back(obj2Dpipe_);
	}
#pragma endregion

}

GPipeline* PipelineManager::GetPipeline(const std::string& name, GPipeline::BlendMord blend)
{
	if (name == "Model") {
		return modelPipeline[blend];
	}
	else if (name == "Obj2D") {
		return obj2DPipeline[blend];
	}
	return nullptr;
}
