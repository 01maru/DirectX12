#pragma once
#include "GPipeline.h"
#include <vector>
#include <memory>
#include <string>

class PipelineManager
{
private:
#pragma region Sprite
	std::unique_ptr<GPipeline> loadingSpritePipe;		//	Loading用

#pragma endregion

#pragma region Model

	std::unique_ptr<GPipeline> modelSilhouettePipe;		//	Obj3D単色シェーダー

#pragma endregion



	std::vector<std::unique_ptr<GPipeline>> modelPipeline;
	std::vector<std::unique_ptr<GPipeline>> obj2DPipeline;
	std::vector<std::unique_ptr<GPipeline>> luminncePipeline;
	std::vector<std::unique_ptr<GPipeline>> postEffectPipeline;
	std::vector<std::unique_ptr<GPipeline>> postShadowPipeline;
	std::vector<std::unique_ptr<GPipeline>> particlePipeline;
	std::vector<std::unique_ptr<GPipeline>> spritePipeline;
	std::vector<std::unique_ptr<GPipeline>> grassPipeline;
	std::unique_ptr<GPipeline> shadowPipeline;
	std::unique_ptr<GPipeline> shadowRecieverPipeline;
	std::unique_ptr<GPipeline> xBlurPipeline;
	std::unique_ptr<GPipeline> yBlurPipeline;
	std::unique_ptr<GPipeline> luminncexBlurPipeline;
	std::unique_ptr<GPipeline> luminnceyBlurPipeline;


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

