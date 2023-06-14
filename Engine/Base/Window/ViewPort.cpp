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
		scissorRect[i].left = left;								// �؂蔲�����W��
		scissorRect[i].right = scissorRect[i].left + width;		// �؂蔲�����W�E
		scissorRect[i].top = top;								// �؂蔲�����W��
		scissorRect[i].bottom = scissorRect[i].top + height;	// �؂蔲�����W��
	}
}

void ViewPort::Update()
{
	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	MyDirectX::GetInstance()->GetCmdList()->RSSetViewports((UINT)viewport.size(), &viewport.front());

	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	MyDirectX::GetInstance()->GetCmdList()->RSSetScissorRects((UINT)scissorRect.size(), &scissorRect.front());
}
