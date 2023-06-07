#include "ViewPort.h"
#include "DirectX.h"

void ViewPort::InitializeVP(const int width, const int height, const int topLftX, const int topLftY, const float MinDepth, const float MaxDepth, int viewportNum)
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
}

void ViewPort::InitializeSR(const int left, const int width, const int top, const int height, int rectNum)
{
	scissorRect.resize(rectNum);

	for (int i = 0; i < rectNum; i++)
	{
		scissorRect[i].left = left;								// 切り抜き座標左
		scissorRect[i].right = scissorRect[i].left + width;		// 切り抜き座標右
		scissorRect[i].top = top;								// 切り抜き座標上
		scissorRect[i].bottom = scissorRect[i].top + height;	// 切り抜き座標下
	}
}

void ViewPort::Update()
{
	// ビューポート設定コマンドを、コマンドリストに積む
	MyDirectX::GetInstance()->GetCmdList()->RSSetViewports((UINT)viewport.size(), &viewport.front());

	// シザー矩形設定コマンドを、コマンドリストに積む
	MyDirectX::GetInstance()->GetCmdList()->RSSetScissorRects((UINT)scissorRect.size(), &scissorRect.front());
}
