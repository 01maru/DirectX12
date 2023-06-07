#include "PipelineManager.h"
#include "Shader.h"

PipelineManager* PipelineManager::GetInstance()
{
	static PipelineManager instance;
	return &instance;
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
		
		GPipeline* modelpipeline_ = modelPipeline.back().get();
		modelpipeline_->Init(objShader, modelInputLayout, _countof(modelInputLayout), 4
			, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK);
		modelpipeline_->SetBlend(i);
	}

	Shader shadowShader(L"Resources/shader/ObjShadowVS.hlsl", L"Resources/shader/ObjShadowPS.hlsl");

	D3D12_INPUT_ELEMENT_DESC shadowInputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
	};

	shadowPipeline = std::make_unique<GPipeline>();
	shadowPipeline->Init(shadowShader, shadowInputLayout, _countof(shadowInputLayout), 4
		, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL, true, DXGI_FORMAT_R32G32_FLOAT);
	shadowPipeline->SetBlend(GPipeline::NONE_BLEND);

	Shader shadowRecieverShader(L"Resources/shader/ShadowRecieverVS.hlsl", L"Resources/shader/ShadowRecieverPS.hlsl");

	D3D12_INPUT_ELEMENT_DESC shadowRecieverInputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	法線ベクトル
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};

	shadowRecieverPipeline = std::make_unique<GPipeline>();
	shadowRecieverPipeline->Init(shadowRecieverShader, shadowRecieverInputLayout, _countof(shadowRecieverInputLayout)
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL, true, DXGI_FORMAT_R11G11B10_FLOAT, 2);
	shadowRecieverPipeline->SetBlend(GPipeline::NONE_BLEND);
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
		obj2Dpipe_->Init(test2dShader, inputLayout2D, _countof(inputLayout2D), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);
		obj2Dpipe_->SetBlend(i);

		//	pipeline追加
		obj2DPipeline.emplace_back(obj2Dpipe_);
	}
#pragma endregion

#pragma region PostEffect
	postEffectPipeline.reserve(blendMordNum);
	Shader luminnceShader(L"Resources/shader/ScreenVS.hlsl", L"Resources/shader/ScreenPS.hlsl");
	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* postEffectpipe_ = new GPipeline();
		postEffectpipe_->Init(luminnceShader, inputLayout2D, _countof(inputLayout2D), 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK,D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 4);
		postEffectpipe_->SetBlend(i);

		//	pipeline追加
		postEffectPipeline.emplace_back(postEffectpipe_);
	}

	Shader postEffect(L"Resources/shader/ScreenVS.hlsl", L"Resources/shader/ScreenShadowPS.hlsl");
	postShadowPipeline.reserve(blendMordNum);
	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* postEffectpipe_ = new GPipeline();
		postEffectpipe_->Init(postEffect, inputLayout2D, _countof(inputLayout2D), 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK,D3D12_DEPTH_WRITE_MASK_ZERO);
		postEffectpipe_->SetBlend(i);

		//	pipeline追加
		postShadowPipeline.emplace_back(postEffectpipe_);
	}

	Shader luminncePostEffect(L"Resources/shader/ScreenVS.hlsl", L"Resources/shader/LuminncePS.hlsl");
	luminncePipeline.reserve(blendMordNum);
	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* postEffectpipe_ = new GPipeline();
		postEffectpipe_->Init(luminncePostEffect, inputLayout2D, _countof(inputLayout2D), 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK,D3D12_DEPTH_WRITE_MASK_ZERO);
		postEffectpipe_->SetBlend(i);

		//	pipeline追加
		luminncePipeline.emplace_back(postEffectpipe_);
	}


#pragma region Blur
	Shader xblur(L"Resources/shader/XBlurVS.hlsl", L"Resources/shader/BlurPS.hlsl");

	xBlurPipeline = std::make_unique<GPipeline>();
	xBlurPipeline->Init(xblur, inputLayout2D, _countof(inputLayout2D)
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R32G32_FLOAT);

	Shader yblur(L"Resources/shader/YBlurVS.hlsl", L"Resources/shader/BlurPS.hlsl");

	yBlurPipeline = std::make_unique<GPipeline>();
	yBlurPipeline->Init(yblur, inputLayout2D, _countof(inputLayout2D)
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R32G32_FLOAT);

	luminncexBlurPipeline = std::make_unique<GPipeline>();
	luminncexBlurPipeline->Init(xblur, inputLayout2D, _countof(inputLayout2D)
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true);

	luminnceyBlurPipeline = std::make_unique<GPipeline>();
	luminnceyBlurPipeline->Init(yblur, inputLayout2D, _countof(inputLayout2D)
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true);
#pragma endregion
#pragma endregion

#pragma region Sprite
	spritePipeline.reserve(blendMordNum);

	D3D12_INPUT_ELEMENT_DESC spriteInputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};

	Shader Spriteshader(L"Resources/shader/SpriteVS.hlsl", L"Resources/shader/SpritePS.hlsl");

	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* spritepipe_ = new GPipeline();
		spritepipe_->Init(Spriteshader, spriteInputLayout, _countof(spriteInputLayout), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);
		spritepipe_->SetBlend(i);

		//	pipeline追加
		spritePipeline.emplace_back(spritepipe_);
	}
#pragma endregion

#pragma region Particle
	particlePipeline.reserve(blendMordNum);

	D3D12_INPUT_ELEMENT_DESC particleInputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
	};
	Shader particleShader(L"Resources/shader/ParticleVS.hlsl", L"Resources/shader/ParticlePS.hlsl", "main", L"Resources/shader/ParticleGS.hlsl");

	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* particlepipe_ = new GPipeline();
		particlepipe_->Init(particleShader, particleInputLayout, _countof(particleInputLayout), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK,D3D12_DEPTH_WRITE_MASK_ZERO);
		particlepipe_->SetBlend(i);

		//	pipeline追加
		particlePipeline.emplace_back(particlepipe_);
	}
#pragma endregion

#pragma region Grass
	grassPipeline.reserve(blendMordNum);

	Shader grassShader(L"Resources/shader/GrassParticleVS.hlsl", L"Resources/shader/GrassParticlePS.hlsl", "main", L"Resources/shader/GrassParticleGS.hlsl");

	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* particlepipe_ = new GPipeline();
		particlepipe_->Init(grassShader, particleInputLayout, _countof(particleInputLayout), 3, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK);
		particlepipe_->SetBlend(i);

		//	pipeline追加
		grassPipeline.emplace_back(particlepipe_);
	}
#pragma endregion
}

GPipeline* PipelineManager::GetPipeline(const std::string& name, GPipeline::BlendMord blend)
{
	if (name == "Model") {
		return modelPipeline[blend].get();
	}
	else if (name == "Obj2D") {
		return obj2DPipeline[blend].get();
	}
	else if (name == "PostEffect") {
		return postEffectPipeline[blend].get();
	}
	else if (name == "Luminnce") {
		return luminncePipeline[blend].get();
	}
	else if (name == "PostEffectShadow") {
		return postShadowPipeline[blend].get();
	}
	else if (name == "Sprite") {
		return spritePipeline[blend].get();
	}
	else if (name == "Particle") {
		return particlePipeline[blend].get();
	}
	else if (name == "GrassParticle") {
		return grassPipeline[blend].get();
	}
	else if (name == "Shadow") {
		return shadowPipeline.get();
	}
	else if (name == "ShadowReciever") {
		return shadowRecieverPipeline.get();
	}
	else if (name == "xBlur") {
		return xBlurPipeline.get();
	}
	else if (name == "yBlur") {
		return yBlurPipeline.get();
	}
	else if (name == "luminncexBlur") {
		return luminncexBlurPipeline.get();
	}
	else if (name == "luminnceyBlur") {
		return luminnceyBlurPipeline.get();
	}
	return nullptr;
}
