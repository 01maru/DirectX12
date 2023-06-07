#include "SpriteCommon.h"
#include "DirectX.h"
#include "PipelineManager.h"

SpriteCommon::SpriteCommon()
{
	Initialize();
}

SpriteCommon* SpriteCommon::GetInstance()
{
	static SpriteCommon instance;
	return &instance;
}

void SpriteCommon::Initialize()
{
	mat2D.Identity();
	mat2D.m[0][0] = 2.0f / Window::window_width;
	mat2D.m[1][1] = -2.0f / Window::window_height;
	mat2D.m[3][0] = -1;
	mat2D.m[3][1] = 1;
}

void SpriteCommon::Draw()
{
	GPipeline* pipeline_ = PipelineManager::GetInstance()->GetPipeline("Sprite", GPipeline::ALPHA_BLEND);
	pipeline_->Setting();
	pipeline_->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}
