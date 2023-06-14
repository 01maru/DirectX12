#pragma once
#include <d3d12.h>
#include <vector>

class ViewPort
{
private:
	std::vector<D3D12_VIEWPORT> viewport;
	std::vector<D3D12_RECT> scissorRect;
public:
	ViewPort() {};

	void InitializeVP(const int width, const int height, const int topLftX, const int topLftY, const float MinDepth = 0.0f, const float MaxDepth = 1.0f, int viewportNum = 1);
	void InitializeSR(const int left, const int width, const int top, const int height, int rectNum = 1);
	void Update();
};

