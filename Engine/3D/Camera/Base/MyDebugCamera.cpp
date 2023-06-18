#include "MyDebugCamera.h"
#include "InputManager.h"
#include "MyMath.h"
#include <cmath>

void MyDebugCamera::Initialize(Vector3D eye, Vector3D target, Vector3D up)
{
	SetProjectionMatrix(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, MyMath::ConvertToRad(90.0f));

	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	//	disEyeTraget������
	frontVec_ = target - eye;
	disEyeTarget = frontVec_.GetLength();

	//	�����x�N�g��
	CalcDirectionVec();
}

void MyDebugCamera::Update()
{
	InputKeyboard* keyboard = InputManager::GetInstance()->GetKeyboard();
	InputMouse* mouse = InputManager::GetInstance()->GetMouse();

	Vector2D moveCursor = mouse->GetCursor() - mouse->GetPrevCursor();
	float cursorDisPrev = moveCursor.GetLength();
	moveCursor.Normalize();

#pragma region SetMode
	if (mouse->GetClickTrigger(InputMouse::WheelClick)) {
		if (keyboard->GetKey(DIK_LSHIFT) || keyboard->GetKey(DIK_RSHIFT))	//	shift��������Ă���
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
	disEyeTarget -= mouse->GetWheel() * (disEyeTarget * 0.001f);
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
		if (mouse->GetClick(InputMouse::WheelClick)) {
			//	���E�ړ�
			target_ -= rightVec_ * (float)(moveCursor.x) * spd;
			//	�㉺�ړ�
			target_ -= downVec_ * (float)(moveCursor.y) * spd;
		}
		break;
	case MyDebugCamera::RotationMove:
		if (mouse->GetClick(InputMouse::WheelClick)) {
			moveCursor /= 1000;
			moveCursor *= cursorDisPrev;
			if (up_.y < 0) {
				moveCursor.x = -moveCursor.x;
			}
			cursorPos += moveCursor;
		}
		break;
	default:
		break;
	}
	//	�O��ړ�
	target_ += -frontVec_ * (float)(keyboard->GetKey(DIK_Z) - keyboard->GetKey(DIK_X)) * spd;

	//	�͈́@0�@>�@cursorPos�@>�@PIx2�@�ɐݒ�
	if (cursorPos.x >= MyMath::PIx2) cursorPos.x -= MyMath::PIx2;
	if (cursorPos.x < 0) cursorPos.x += MyMath::PIx2;
	if (cursorPos.y >= MyMath::PIx2) cursorPos.y -= MyMath::PIx2;
	if (cursorPos.y < 0) cursorPos.y += MyMath::PIx2;

	//	������x�N�g���Ǝ��_���W�X�V
	up_.y = cosf(cursorPos.y);
	eye_.x = target_.x - disEyeTarget * cosf(cursorPos.y) * sinf(cursorPos.x);
	eye_.y = target_.y + disEyeTarget * sinf(cursorPos.y);
	eye_.z = target_.z - disEyeTarget * cosf(cursorPos.y) * cosf(cursorPos.x);

	//	�����x�N�g��
	CalcDirectionVec();

	//	�r���{�[�h
	CalcBillboard();

	MatUpdate();
}
