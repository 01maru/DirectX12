#pragma once
#include <d3d12.h>
#include <vector>

class ViewPortScissorRect
{
private:
	std::vector<D3D12_VIEWPORT> viewport_;
	std::vector<D3D12_RECT> scissorRect_;
public:
	ViewPortScissorRect() {};

	void InitializeVP(int width, int height, int topLftX, int topLftY, float MinDepth = 0.0f, float MaxDepth = 1.0f, int viewportNum = 1);
	void InitializeSR(int left, int width, int top, int height, int rectNum = 1);
	void RSSetVPandSR();
};

