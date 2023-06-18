#pragma once
#include "GPipeline.h"
#include <vector>
#include <memory>
#include <string>

class PipelineManager
{
private:
#pragma region Sprite

	std::vector<std::unique_ptr<GPipeline>> spritePipeline_;
	std::unique_ptr<GPipeline> loadingSpritePipe_;		//	Loading用

#pragma endregion

#pragma region Model

	std::unique_ptr<GPipeline> modelSilhouettePipe_;		//	Obj3D単色シェーダー
	std::vector<std::unique_ptr<GPipeline>> modelPipeline_;
	std::unique_ptr<GPipeline> shadowPipeline_;
	std::unique_ptr<GPipeline> shadowRecieverPipeline_;

#pragma endregion

#pragma region Particle

	std::vector<std::unique_ptr<GPipeline>> particlePipeline_;
	
#pragma endregion

#pragma region PostEffect

	std::unique_ptr<GPipeline> postEffectPipeline_;
	std::unique_ptr<GPipeline> postShadowPipeline_;
	std::unique_ptr<GPipeline> luminncePipeline_;
	std::unique_ptr<GPipeline> xBlurPipeline_;
	std::unique_ptr<GPipeline> yBlurPipeline_;
	std::unique_ptr<GPipeline> luminncexBlurPipeline_;
	std::unique_ptr<GPipeline> luminnceyBlurPipeline_;

#pragma endregion

private:
	void InitializeSprite();
	void InitializeParticle();
	void InitializePostEffect();
	void InitializeModel();

	PipelineManager() {};
	~PipelineManager() {};
public:
	static PipelineManager* GetInstance();
	PipelineManager(const PipelineManager& obj) = delete;
	PipelineManager& operator=(const PipelineManager& obj) = delete;

	void Initialize();

	//	Getter
	GPipeline* GetPipeline(const std::string& name, GPipeline::BlendMord blend = GPipeline::NONE_BLEND);
};

