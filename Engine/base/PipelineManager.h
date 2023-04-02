#pragma once
#include "GPipeline.h"
#include <vector>

class PipelineManager
{
private:
	std::vector<GPipeline*> modelPipeline;
	std::vector<GPipeline*> obj2DPipeline;

	PipelineManager() {};
public:
	~PipelineManager();

	static PipelineManager* GetInstance();
	static void DeleteInstance();
	PipelineManager(const PipelineManager& obj) = delete;
	PipelineManager& operator=(const PipelineManager& obj) = delete;

	void Initialize();

	//	Getter
	GPipeline* GetPipeline(const std::string& name, GPipeline::BlendMord blend);
};

