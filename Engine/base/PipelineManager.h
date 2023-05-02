#pragma once
#include "GPipeline.h"
#include <vector>

class PipelineManager
{
private:
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
	static void DeleteInstance();
	PipelineManager(const PipelineManager& obj) = delete;
	PipelineManager& operator=(const PipelineManager& obj) = delete;

	void Initialize();

	//	Getter
	GPipeline* GetPipeline(const std::string& name, GPipeline::BlendMord blend = GPipeline::NONE_BLEND);
};

