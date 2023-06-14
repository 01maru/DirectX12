#include "InputMouse.h"
#include "Window.h"
#include <cassert>

void InputMouse::Initialize()
{
	Window* win = Window::GetInstance();

	HRESULT result = DirectInput8Create(
		win->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	//	デバイス生成(キーボード以外も可能)
	result = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));

	//	入力形成のセット
	result = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));

	//	排他制御のレベルセット
	result = mouse_->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void InputMouse::SetInputInfo()
{
	//	マウス全体の情報
	mouse_->Acquire();
	mouse_->Poll();
	mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &click_);

	//	カーソルの情報
	POINT cursor;
	GetCursorPos(&cursor);

	cursor_.x = (float)cursor.x;
	cursor_.y = (float)cursor.y;

	if (isRockCursor_) {
		RockCursor();
	}
	else {
		//	範囲指定しない
		ClipCursor(NULL);
		ScreenToClient(Window::GetInstance()->GetHwnd(), &cursor);
	}
}

void InputMouse::Update()
{
	//	前フレームの情報
	prevClick_ = click_;
	prevCursor_ = cursor_;

	SetInputInfo();
}

void InputMouse::RockCursor()
{
	RECT rec;
	//	ウィンドウの大きさ取得
	GetWindowRect(Window::GetInstance()->GetHwnd(), &rec);

	Vector2D center((float)(rec.right + rec.left) / 2.0f, (float)(rec.bottom + rec.top) / 2.0f);
	float width  = Window::window_width / 2.0f;
	float height = Window::window_height / 2.0f;
	rec.left   = (LONG)(center.x - width);
	rec.right  = (LONG)(center.x + width);
	rec.top    = (LONG)(center.y - height);
	rec.bottom = (LONG)(center.y + height);

	prevCursor_ = center;

	SetCursorPos((int)center.x, (int)center.y);
	//	カーソル表示しない
	ShowCursor(false);
	//	範囲指定
	ClipCursor(&rec);
}

bool InputMouse::Click(int type)
{
	return (click_.rgbButtons[type] & (0x80));

}

bool InputMouse::ClickTrigger(int type)
{
	return (click_.rgbButtons[type] & (0x80)) && !(prevClick_.rgbButtons[type] & (0x80));
}

//void InputMouse::SetCursorPos(Vector2D& pos)
//{
//	pos = cursor;
//}

LONG InputMouse::Wheel()
{
	return click_.lZ;
}
