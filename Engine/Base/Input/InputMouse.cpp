#include "InputMouse.h"
#include "Window.h"
#include <cassert>

#include "ImGuiManager.h"

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

void InputMouse::LockCursor()
{
	if (!isLockCursor_) return;

	RECT rec;
	//	ウィンドウの大きさ取得
	GetWindowRect(Window::GetInstance()->GetHwnd(), &rec);

	Vector2D center((float)(rec.right + rec.left) / 2.0f, (float)(rec.bottom + rec.top) / 2.0f);
	float width = Window::sWIN_WIDTH / 2.0f;
	float height = Window::sWIN_HEIGHT / 2.0f;
	rec.left = (LONG)(center.x - width);
	rec.right = (LONG)(center.x + width);
	rec.top = (LONG)(center.y - height);
	rec.bottom = (LONG)(center.y + height);

	SetCursorPos((int)center.x, (int)center.y);
	//	範囲指定
	ClipCursor(&rec);

	//	UpdateCursor
	prevCursor_ = center;
	cursor_ = center;
}

void InputMouse::UnLockCursor()
{
	if (isLockCursor_) return;

	//	カーソルの情報
	POINT cursor;
	//	ScreenLeftTop(0, 0)
	GetCursorPos(&cursor);
	//	WindowLeftTop(0, 0)
	ScreenToClient(Window::GetInstance()->GetHwnd(), &cursor);

	//	範囲指定しない
	ClipCursor(NULL);

	//	UpdateCursor
	cursor_.x = (float)cursor.x;
	cursor_.y = (float)cursor.y;
}

void InputMouse::SetInputInfo()
{
	//	マウス全体の情報
	mouse_->Acquire();
	mouse_->Poll();
	mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &click_);

	LockCursor();
	UnLockCursor();

	cursorMoveLen_ = cursor_ - prevCursor_;
}

void InputMouse::Update()
{
	//	前フレームの情報
	prevClick_ = click_;
	prevCursor_ = cursor_;

	SetInputInfo();

	ShowCursor(showCursor_);
}

void InputMouse::ImGuiUpdateCursor(ImGuiManager* imgui)
{
	if (!imgui->TreeNode("Cursor")) return;

	imgui->Text("CursorPos  : (%.f, %.f)", cursor_.x, cursor_.y);
	imgui->Text("CursorMove : (%d, %d)", cursorMoveLen_.x, cursorMoveLen_.y);

	imgui->Text("ShowCursor : %s", showCursor_ ? "True" : "False");

	imgui->Text("LockCursor : %s", isLockCursor_ ? "True" : "False");

	imgui->TreePop();
}

void InputMouse::ImGuiUpdateClick(ImGuiManager* imgui)
{
	if (!imgui->TreeNode("Click")) return;

	imgui->Text("LeftClick  : %s\nTrigger : %s\nRelease : %s\n"
		, GetClick(LeftClick) ? "True" : "False"
		, GetClickTrigger(LeftClick) ? "True" : "False"
		, GetClickRelease(LeftClick) ? "True" : "False");

	imgui->Text("RightClick  : %s\nTrigger : %s\nRelease : %s\n"
		, GetClick(RightClick) ? "True" : "False"
		, GetClickTrigger(RightClick) ? "True" : "False"
		, GetClickRelease(RightClick) ? "True" : "False");

	imgui->Text("WheelClick  : %s\nTrigger : %s\nRelease : %s\n"
		, GetClick(WheelClick) ? "True" : "False"
		, GetClickTrigger(WheelClick) ? "True" : "False"
		, GetClickRelease(WheelClick) ? "True" : "False");

	imgui->Text("SideClick  : %s\nTrigger : %s\nRelease : %s\n"
		, GetClick(SideClick) ? "True" : "False"
		, GetClickTrigger(SideClick) ? "True" : "False"
		, GetClickRelease(SideClick) ? "True" : "False");

	imgui->Text("WheelMove  : %d", click_.lZ);

	imgui->TreePop();
}

void InputMouse::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (!imgui->TreeNode("Mouse")) return;

	ImGuiUpdateCursor(imgui);

	ImGuiUpdateClick(imgui);

	imgui->TreePop();
}

bool InputMouse::GetClick(MouseButton type)
{
	return (click_.rgbButtons[type] & (0x80));
}

bool InputMouse::GetClickTrigger(MouseButton type)
{
	return (click_.rgbButtons[type] & (0x80)) && !(prevClick_.rgbButtons[type] & (0x80));
}

bool InputMouse::GetClickRelease(MouseButton type)
{
	return !(click_.rgbButtons[type] & (0x80)) && (prevClick_.rgbButtons[type] & (0x80));
}
