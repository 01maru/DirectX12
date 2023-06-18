#include "ViewPortScissorRect.h"
#include "DirectX.h"

void ViewPortScissorRect::InitializeVP(int width, int height, int topLftX, int topLftY, float MinDepth, float MaxDepth, int viewportNum)
{
	viewport_.resize(viewportNum);

	for (int i = 0; i < viewportNum; i++)
	{
		viewport_[i].Width = (FLOAT)width;
		viewport_[i].Height = (FLOAT)height;
		viewport_[i].TopLeftX = (FLOAT)topLftX;
		viewport_[i].TopLeftY = (FLOAT)topLftY;
		viewport_[i].MinDepth = MinDepth;
		viewport_[i].MaxDepth = MaxDepth;
	}
}

void ViewPortScissorRect::InitializeSR(int left, int width, int top, int height, int rectNum)
{
	scissorRect_.resize(rectNum);

	for (int i = 0; i < rectNum; i++)
	{
		scissorRect_[i].left = left;								// 切り抜き座標左
		scissorRect_[i].right = scissorRect_[i].left + width;		// 切り抜き座標右
		scissorRect_[i].top = top;								// 切り抜き座標上
		scissorRect_[i].bottom = scissorRect_[i].top + height;	// 切り抜き座標下
	}
}

void ViewPortScissorRect::RSSetVPandSR()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	// ビューポート設定コマンドを、コマンドリストに積む
	cmdList->RSSetViewports((UINT)viewport_.size(), &viewport_.front());

	// シザー矩形設定コマンドを、コマンドリストに積む
	cmdList->RSSetScissorRects((UINT)scissorRect_.size(), &scissorRect_.front());
}
