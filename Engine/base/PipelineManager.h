#pragma once
#include "GPipeline.h"
#include <vector>

class PipelineManager
{
private:
	std::vector<std::unique_ptr<GPipeline>> modelPipeline;
	std::vector<std::unique_ptr<GPipeline>> obj2DPipeline;
	std::vector<std::unique_ptr<GPipeline>> postEffectPipeline;
	std::vector<std::unique_ptr<GPipeline>> particlePipeline;
	std::vector<std::unique_ptr<GPipeline>> grassPipeline;

	PipelineManager() {};
	~PipelineManager() {};
public:
	static PipelineManager* GetInstance();
	static void DeleteInstance();
	PipelineManager(const PipelineManager& obj) = delete;
	PipelineManager& operator=(const PipelineManager& obj) = delete;

	void Initialize();

	//	Getter
	GPipeline* GetPipeline(const std::string& name, GPipeline::BlendMord blend);
};

