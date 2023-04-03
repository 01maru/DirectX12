#include "ScissorRect.h"
#include "DirectX.h"

void ScissorRect::Init(const int left, const int width, const int top, const int height, int rectNum)
{
	scissorRect.resize(rectNum);

	for (int i = 0; i < rectNum; i++)
	{
		scissorRect[i].left = left;							// 切り抜き座標左
		scissorRect[i].right = scissorRect[i].left + width;	// 切り抜き座標右
		scissorRect[i].top = top;							// 切り抜き座標上
		scissorRect[i].bottom = scissorRect[i].top + height;	// 切り抜き座標下
	}
}

void ScissorRect::Update()
{
	// シザー矩形設定コマンドを、コマンドリストに積む
	MyDirectX::GetInstance()->GetCmdList()->RSSetScissorRects((UINT)scissorRect.size(), &scissorRect.front());
}
