#include "ViewPort.h"
#include "DirectX.h"

ViewPort::ViewPort()
{
}

void ViewPort::Init(const int width, const int height, const int topLftX, const int topLftY, const float MinDepth, const float MaxDepth, int viewportNum)
{
	viewport.resize(viewportNum);

	for (int i = 0; i < viewportNum; i++)
	{
		viewport[i].Width = (FLOAT)width;
		viewport[i].Height = (FLOAT)height;
		viewport[i].TopLeftX = (FLOAT)topLftX;
		viewport[i].TopLeftY = (FLOAT)topLftY;
		viewport[i].MinDepth = MinDepth;
		viewport[i].MaxDepth = MaxDepth;
	}

	mat.m[0][0] = viewport[0].Width / 2;
	mat.m[1][1] = -viewport[0].Height / 2;
	mat.m[3][0] = viewport[0].Width / 2;
	mat.m[3][1] = viewport[0].Height / 2;
}

void ViewPort::Update()
{
	// ビューポート設定コマンドを、コマンドリストに積む
	MyDirectX::GetInstance()->GetCmdList()->RSSetViewports((UINT)viewport.size(), &viewport.front());
}
