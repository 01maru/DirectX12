#include "MyDebugCamera.h"
#include "InputManager.h"
#include "MyMath.h"
#include <cmath>

void MyDebugCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	SetProjectionMatrix(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, MyMath::ConvertToRad(90.0f));

	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	//	disEyeTraget初期化
	frontVec_ = target - eye;
	disEyeTarget_ = frontVec_.GetLength();

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
			mode_ = TranslationMove;
		}
		else {
			//	注視点周りを回転移動
			mode_ = RotationMove;
		}
	}
#pragma endregion

#pragma region SetDisEyeTarget
	//	キー入力
	disEyeTarget_ -= mouse->GetWheel() * (disEyeTarget_ * 0.001f);
	//	範囲設定
	float minDis_ = 10.0f;	//	最小値
	disEyeTarget_ = MyMath::mMax(disEyeTarget_, minDis_);
#pragma endregion

	//	注視点更新
	float spd = 0.1f;
	switch (mode_)
	{
	case MyDebugCamera::NoMove:
		break;
	case MyDebugCamera::TranslationMove:
		if (mouse->GetClick(InputMouse::WheelClick)) {
			//	左右移動
			target_ -= rightVec_ * (float)(moveCursor.x) * spd;
			//	上下移動
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
			cursorPos_ += moveCursor;
		}
		break;
	default:
		break;
	}
	//	前後移動
	target_ += -frontVec_ * (float)(keyboard->GetKey(DIK_Z) - keyboard->GetKey(DIK_X)) * spd;

	//	範囲　0　>　cursorPos　>　PIx2　に設定
	if (cursorPos_.x >= MyMath::PIx2) cursorPos_.x -= MyMath::PIx2;
	if (cursorPos_.x < 0) cursorPos_.x += MyMath::PIx2;
	if (cursorPos_.y >= MyMath::PIx2) cursorPos_.y -= MyMath::PIx2;
	if (cursorPos_.y < 0) cursorPos_.y += MyMath::PIx2;

	//	上方向ベクトルと視点座標更新
	up_.y = cosf(cursorPos_.y);
	eye_.x = target_.x - disEyeTarget_ * cosf(cursorPos_.y) * sinf(cursorPos_.x);
	eye_.y = target_.y + disEyeTarget_ * sinf(cursorPos_.y);
	eye_.z = target_.z - disEyeTarget_ * cosf(cursorPos_.y) * cosf(cursorPos_.x);

	//	方向ベクトル
	CalcDirectionVec();

	//	ビルボード
	CalcBillboard();

	MatUpdate();
}
