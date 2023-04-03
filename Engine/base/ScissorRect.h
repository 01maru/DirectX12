#pragma once
#include <d3d12.h>
#include <vector>

class ScissorRect
{
private:
	std::vector<D3D12_RECT> scissorRect;
public:
	ScissorRect() {};
	void Init(const int left, const int width, const int top, const int height, int rectNum = 1);
	void Update();
};

