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

	//	disEyeTraget初期化
	frontVec = target - eye;
	disEyeTarget = frontVec.GetLength();

	//	方向ベクトル
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
	//	キー入力
	disEyeTarget += -input->Wheel() * (disEyeTarget * 0.001f);
	//	範囲設定
	float minDis_ = 10.0f;	//	最小値
	disEyeTarget = MyMath::mMax(disEyeTarget, minDis_);
#pragma endregion

	//	注視点更新
	float spd = 0.1f;
	switch (mode)
	{
	case MyDebugCamera::NoMove:
		break;
	case MyDebugCamera::TranslationMove:
		if (input->Click(Input::WheelClick)) {
			//	左右移動
			target -= rightVec * (float)(moveCursor.x) * spd;
			//	上下移動
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
	//	前後移動
	target += -frontVec * (float)(input->GetKey(DIK_Z) - input->GetKey(DIK_X)) * spd;

	//	範囲　0　>　cursorPos　>　PIx2　に設定
	if (cursorPos.x >= MyMath::PIx2) cursorPos.x -= MyMath::PIx2;
	if (cursorPos.x < 0) cursorPos.x += MyMath::PIx2;
	if (cursorPos.y >= MyMath::PIx2) cursorPos.y -= MyMath::PIx2;
	if (cursorPos.y < 0) cursorPos.y += MyMath::PIx2;

	//	上方向ベクトルと視点座標更新
	up.y = cosf(cursorPos.y);
	eye.x = target.x - disEyeTarget * cosf(cursorPos.y) * sinf(cursorPos.x);
	eye.y = target.y + disEyeTarget * sinf(cursorPos.y);
	eye.z = target.z - disEyeTarget * cosf(cursorPos.y) * cosf(cursorPos.x);

	//	方向ベクトル
	CalcDirectionVec();

	//	ビルボード
	CalcBillboard();

	MatUpdate();
}
