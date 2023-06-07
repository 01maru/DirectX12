#include "MyDebugCamera.h"
#include "Input.h"
#include "MyMath.h"
#include <cmath>

void MyDebugCamera::Initialize(Vector3D eye_, Vector3D target_, Vector3D up_)
{
	SetProjectionMatrix(Window::window_width, Window::window_height, MyMath::ConvertToRad(90.0f));

	eye = eye_;
	target = target_;
	up = up_;

	MatUpdate();

	//	disEyeTraget������
	frontVec = target - eye;
	disEyeTarget = frontVec.GetLength();

	//	�����x�N�g��
	CalcDirectionVec();
}

void MyDebugCamera::Update()
{
	Input* input = Input::GetInstance();

	Vector2D moveCursor = input->GetCursor() - input->GetPrevCursor();
	float cursorDisPrev = moveCursor.GetLength();
	moveCursor.Normalize();

#pragma region SetMode
	if (input->ClickTrigger(Input::WheelClick)) {
		if (input->GetKey(DIK_LSHIFT) || input->GetKey(DIK_RSHIFT))	//	shift��������Ă���
		{
			//	���s�ړ�
			mode = TranslationMove;
		}
		else {
			//	�����_�������]�ړ�
			mode = RotationMove;
		}
	}
#pragma endregion

#pragma region SetDisEyeTarget
	//	�L�[����
	disEyeTarget += -input->Wheel() * (disEyeTarget * 0.001f);
	//	�͈͐ݒ�
	float minDis_ = 10.0f;	//	�ŏ��l
	disEyeTarget = MyMath::mMax(disEyeTarget, minDis_);
#pragma endregion

	//	�����_�X�V
	float spd = 0.1f;
	switch (mode)
	{
	case MyDebugCamera::NoMove:
		break;
	case MyDebugCamera::TranslationMove:
		if (input->Click(Input::WheelClick)) {
			//	���E�ړ�
			target -= rightVec * (float)(moveCursor.x) * spd;
			//	�㉺�ړ�
			target -= downVec * (float)(moveCursor.y) * spd;
		}
		break;
	case MyDebugCamera::RotationMove:
		if (input->Click(Input::WheelClick)) {
			moveCursor /= 1000;
			moveCursor *= cursorDisPrev;
			if (up.y < 0) {
				moveCursor.x = -moveCursor.x;
			}
			cursorPos += moveCursor;
		}
		break;
	default:
		break;
	}
	//	�O��ړ�
	target += -frontVec * (float)(input->GetKey(DIK_Z) - input->GetKey(DIK_X)) * spd;

	//	�͈́@0�@>�@cursorPos�@>�@PIx2�@�ɐݒ�
	if (cursorPos.x >= MyMath::PIx2) cursorPos.x -= MyMath::PIx2;
	if (cursorPos.x < 0) cursorPos.x += MyMath::PIx2;
	if (cursorPos.y >= MyMath::PIx2) cursorPos.y -= MyMath::PIx2;
	if (cursorPos.y < 0) cursorPos.y += MyMath::PIx2;

	//	������x�N�g���Ǝ��_���W�X�V
	up.y = cosf(cursorPos.y);
	eye.x = target.x - disEyeTarget * cosf(cursorPos.y) * sinf(cursorPos.x);
	eye.y = target.y + disEyeTarget * sinf(cursorPos.y);
	eye.z = target.z - disEyeTarget * cosf(cursorPos.y) * cosf(cursorPos.x);

	//	�����x�N�g��
	CalcDirectionVec();

	//	�r���{�[�h
	CalcBillboard();

	MatUpdate();
}
