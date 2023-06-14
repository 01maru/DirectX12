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

	Shader objShader(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");

	D3D12_INPUT_ELEMENT_DESC modelInputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	�@���x�N�g��
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv���W
		{"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};

	Shader silhouetteShader(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/SilhouettePS.hlsl");

	modelSilhouettePipe = std::make_unique<GPipeline>();
	modelSilhouettePipe->Init(silhouetteShader, modelInputLayout, _countof(modelInputLayout), 5,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL, true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	modelSilhouettePipe->SetBlend(GPipeline::ALPHA_BLEND);

	for (int i = 0; i < blendMordNum; i++)
	{
		//	pipeline�ǉ�
		modelPipeline.emplace_back(new GPipeline());
		
		GPipeline* modelpipeline_ = modelPipeline.back().get();
		modelpipeline_->Init(objShader, modelInputLayout, _countof(modelInputLayout), 5
			, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK);
		modelpipeline_->SetBlend(i);
	}

	Shader shadowShader(L"Resources/Shader/ObjShadowVS.hlsl", L"Resources/Shader/ObjShadowPS.hlsl");

	D3D12_INPUT_ELEMENT_DESC shadowInputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
	};

	shadowPipeline = std::make_unique<GPipeline>();
	shadowPipeline->Init(shadowShader, shadowInputLayout, _countof(shadowInputLayout), 5
		, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL, true, DXGI_FORMAT_R32G32_FLOAT);
	shadowPipeline->SetBlend(GPipeline::NONE_BLEND);

	Shader shadowRecieverShader(L"Resources/Shader/ShadowRecieverVS.hlsl", L"Resources/Shader/ShadowRecieverPS.hlsl");

	D3D12_INPUT_ELEMENT_DESC shadowRecieverInputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	�@���x�N�g��
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv���W
	};

	shadowRecieverPipeline = std::make_unique<GPipeline>();
	shadowRecieverPipeline->Init(shadowRecieverShader, shadowRecieverInputLayout, _countof(shadowRecieverInputLayout)
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ALL, true, DXGI_FORMAT_R11G11B10_FLOAT, 2);
	shadowRecieverPipeline->SetBlend(GPipeline::NONE_BLEND);
#pragma endregion

#pragma region Obj2D
	obj2DPipeline.reserve(blendMordNum);
	Shader test2dShader(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");

	D3D12_INPUT_ELEMENT_DESC inputLayout2D[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv���W
	};

	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* obj2Dpipe_ = new GPipeline();
		obj2Dpipe_->Init(test2dShader, inputLayout2D, _countof(inputLayout2D), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);
		obj2Dpipe_->SetBlend(i);

		//	pipeline�ǉ�
		obj2DPipeline.emplace_back(obj2Dpipe_);
	}
#pragma endregion

#pragma region PostEffect
	postEffectPipeline.reserve(blendMordNum);
	Shader luminnceShader(L"Resources/Shader/ScreenVS.hlsl", L"Resources/Shader/ScreenPS.hlsl");
	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* postEffectpipe_ = new GPipeline();
		postEffectpipe_->Init(luminnceShader, inputLayout2D, _countof(inputLayout2D), 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK,D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 4);
		postEffectpipe_->SetBlend(i);

		//	pipeline�ǉ�
		postEffectPipeline.emplace_back(postEffectpipe_);
	}

	Shader postEffect(L"Resources/Shader/ScreenVS.hlsl", L"Resources/Shader/ScreenShadowPS.hlsl");
	postShadowPipeline.reserve(blendMordNum);
	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* postEffectpipe_ = new GPipeline();
		postEffectpipe_->Init(postEffect, inputLayout2D, _countof(inputLayout2D), 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK,D3D12_DEPTH_WRITE_MASK_ZERO);
		postEffectpipe_->SetBlend(i);

		//	pipeline�ǉ�
		postShadowPipeline.emplace_back(postEffectpipe_);
	}

	Shader luminncePostEffect(L"Resources/Shader/ScreenVS.hlsl", L"Resources/Shader/LuminncePS.hlsl");
	luminncePipeline.reserve(blendMordNum);
	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* postEffectpipe_ = new GPipeline();
		postEffectpipe_->Init(luminncePostEffect, inputLayout2D, _countof(inputLayout2D), 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK,D3D12_DEPTH_WRITE_MASK_ZERO);
		postEffectpipe_->SetBlend(i);

		//	pipeline�ǉ�
		luminncePipeline.emplace_back(postEffectpipe_);
	}


#pragma region Blur
	Shader xblur(L"Resources/Shader/XBlurVS.hlsl", L"Resources/Shader/BlurPS.hlsl");

	xBlurPipeline = std::make_unique<GPipeline>();
	xBlurPipeline->Init(xblur, inputLayout2D, _countof(inputLayout2D)
		, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R32G32_FLOAT);

	Shader yblur(L"Resources/Shader/YBlurVS.hlsl", L"Resources/Shader/BlurPS.hlsl");

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
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv���W
	};

	Shader Spriteshader(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");

	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* spritepipe_ = new GPipeline();
		spritepipe_->Init(Spriteshader, spriteInputLayout, _countof(spriteInputLayout), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);
		spritepipe_->SetBlend(i);

		//	pipeline�ǉ�
		spritePipeline.emplace_back(spritepipe_);
	}

	loadingSpritePipe = std::make_unique<GPipeline>();
	loadingSpritePipe->Init(Spriteshader, spriteInputLayout, _countof(spriteInputLayout), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	loadingSpritePipe->SetBlend(GPipeline::ALPHA_BLEND);
#pragma endregion

#pragma region Particle
	particlePipeline.reserve(blendMordNum);

	D3D12_INPUT_ELEMENT_DESC particleInputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
	};
	Shader particleShader(L"Resources/Shader/ParticleVS.hlsl", L"Resources/Shader/ParticlePS.hlsl", "main", L"Resources/Shader/ParticleGS.hlsl");

	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* particlepipe_ = new GPipeline();
		particlepipe_->Init(particleShader, particleInputLayout, _countof(particleInputLayout), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK,D3D12_DEPTH_WRITE_MASK_ZERO);
		particlepipe_->SetBlend(i);

		//	pipeline�ǉ�
		particlePipeline.emplace_back(particlepipe_);
	}
#pragma endregion

#pragma region Grass
	grassPipeline.reserve(blendMordNum);

	Shader grassShader(L"Resources/Shader/GrassParticleVS.hlsl", L"Resources/Shader/GrassParticlePS.hlsl", "main", L"Resources/Shader/GrassParticleGS.hlsl");

	for (int i = 0; i < blendMordNum; i++)
	{
		GPipeline* particlepipe_ = new GPipeline();
		particlepipe_->Init(grassShader, particleInputLayout, _countof(particleInputLayout), 3, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT
			, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK);
		particlepipe_->SetBlend(i);

		//	pipeline�ǉ�
		grassPipeline.emplace_back(particlepipe_);
	}
#pragma endregion
}

GPipeline* PipelineManager::GetPipeline(const std::string& name, GPipeline::BlendMord blend)
{
	if (name == "Model") {
		return modelPipeline[blend].get();
	}
	else if (name == "ModelSilhouette") {
		return modelSilhouettePipe.get();
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
	else if (name == "LoadingSprite") {
		return loadingSpritePipe.get();
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
