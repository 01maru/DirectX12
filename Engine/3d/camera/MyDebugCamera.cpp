#include "MyDebugCamera.h"
#include "Input.h"

MyDebugCamera::~MyDebugCamera()
{
}

void MyDebugCamera::Initialize(Vector3D eye_, Vector3D target_, Vector3D up_)
{
	SetProjectionMatrix(Window::window_width, Window::window_height, MyMath::ConvertToRad(90.0f));

	eye = eye_;
	target = target_;
	up = up_;

	MatUpdate();

	frontVec = target - eye;
	disEyeTarget = frontVec.length();
}

void MyDebugCamera::Update()
{
	Input* input = Input::GetInstance();

	moveCursor = input->GetCursor() - input->GetPrevCursor();
	float cursorDisPrev = moveCursor.length();
	moveCursor.normalize();

#pragma region SetMode
	if (input->ClickTrigger(Input::WheelClick)) {
		if (input->GetKey(DIK_LSHIFT) || input->GetKey(DIK_RSHIFT))	//	shiftが押されてたら
		{
			//	平行移動
			mode = TranslationMove;
		}
		else {
			//	注視点周りを回転移動
			mode = RotationMove;
		}
	}
#pragma endregion

#pragma region SetDisEyeTarget
	disEyeTarget += -input->Wheel() * (disEyeTarget * 0.001f);
	if (disEyeTarget < 10) {
		disEyeTarget = 10;
	}
#pragma endregion

	//	move
	switch (mode)
	{
	case MyDebugCamera::NoMove:
		break;
	case MyDebugCamera::TranslationMove:
		if (input->Click(Input::WheelClick)) {
			target -= rightVec * (float)(moveCursor.x);
			target -= downVec * (float)(moveCursor.y);
		}
		break;
	case MyDebugCamera::RotationMove:
		if (input->Click(Input::WheelClick)) {
			moveCursor /= 1000;
			moveCursor *= cursorDisPrev;
			if (up.y < 0) {
				moveCursor.x = -moveCursor.x;
			}
			cursorSpd += moveCursor;
		}
		break;
	default:
		break;
	}
	float spd = 0.1f;
	target += -frontVec * (float)(input->GetKey(DIK_Z) - input->GetKey(DIK_X)) * spd;

#pragma region 方向ベクトル
	CalcDirectionVec();
#pragma endregion
	
	if (rotAngle.x >= MyMath::PIx2) rotAngle.x -= MyMath::PIx2;
	if (rotAngle.x < 0) rotAngle.x += MyMath::PIx2;
	if (rotAngle.y >= MyMath::PIx2) rotAngle.y -= MyMath::PIx2;
	if (rotAngle.y < 0) rotAngle.y += MyMath::PIx2;

	Vector2D angle = rotAngle;
	angle += cursorSpd;
#pragma region ビルボード
	CalcBillboard();
#pragma endregion

	up.y = cosf(angle.y);
	eye.x = target.x - disEyeTarget * cosf(angle.y) * sinf(angle.x);
	eye.y = target.y + disEyeTarget * sinf(angle.y);
	eye.z = target.z - disEyeTarget * cosf(angle.y) * cosf(angle.x);
	MatUpdate();
}
