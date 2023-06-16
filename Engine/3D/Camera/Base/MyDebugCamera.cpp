#include "MyDebugCamera.h"
#include "InputManager.h"
#include "MyMath.h"
#include <cmath>

void MyDebugCamera::Initialize(Vector3D eye_, Vector3D target_, Vector3D up_)
{
	SetProjectionMatrix(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, MyMath::ConvertToRad(90.0f));

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
	InputKeyboard* keyboard = InputManager::GetInstance()->GetKeyboard();
	InputMouse* mouse = InputManager::GetInstance()->GetMouse();

	Vector2D moveCursor = mouse->GetCursor() - mouse->GetPrevCursor();
	float cursorDisPrev = moveCursor.GetLength();
	moveCursor.Normalize();

#pragma region SetMode
	if (mouse->GetClickTrigger(InputMouse::WheelClick)) {
		if (keyboard->GetKey(DIK_LSHIFT) || keyboard->GetKey(DIK_RSHIFT))	//	shiftが押されてたら
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
	disEyeTarget -= mouse->GetWheel() * (disEyeTarget * 0.001f);
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
		if (mouse->GetClick(InputMouse::WheelClick)) {
			//	左右移動
			target -= rightVec * (float)(moveCursor.x) * spd;
			//	上下移動
			target -= downVec * (float)(moveCursor.y) * spd;
		}
		break;
	case MyDebugCamera::RotationMove:
		if (mouse->GetClick(InputMouse::WheelClick)) {
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
	target += -frontVec * (float)(keyboard->GetKey(DIK_Z) - keyboard->GetKey(DIK_X)) * spd;

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
