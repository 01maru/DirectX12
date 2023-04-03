#include "ScissorRect.h"
#include "DirectX.h"

void ScissorRect::Init(const int left, const int width, const int top, const int height, int rectNum)
{
	scissorRect.resize(rectNum);

	for (int i = 0; i < rectNum; i++)
	{
		scissorRect[i].left = left;							// �؂蔲�����W��
		scissorRect[i].right = scissorRect[i].left + width;	// �؂蔲�����W�E
		scissorRect[i].top = top;							// �؂蔲�����W��
		scissorRect[i].bottom = scissorRect[i].top + height;	// �؂蔲�����W��
	}
}

void ScissorRect::Update()
{
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	MyDirectX::GetInstance()->GetCmdList()->RSSetScissorRects((UINT)scissorRect.size(), &scissorRect.front());
}
