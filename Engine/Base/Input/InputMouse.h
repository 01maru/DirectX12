#pragma once
#include <dinput.h>
#include <wrl.h>
#include <stdint.h>
#include "Vector2D.h"

class ImGuiManager;

class InputMouse
{
public:
	enum MouseButton {
		LeftClick = 0,
		RightClick,
		WheelClick,
		SideClick,
	};
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IDirectInputDevice8> mouse_;

	DIMOUSESTATE click_ = {};
	DIMOUSESTATE prevClick_ = {};

	Vector2D cursor_;
	Vector2D prevCursor_;
	Vector2D cursorMoveLen_;
	bool isLockCursor_ = false;
	bool showCursor_ = true;

private:	//	関数
	//	入力の情報設定
	void SetInputInfo();
	void LockCursor();
	void UnLockCursor();

	void ImGuiUpdateCursor(ImGuiManager* imgui);
	void ImGuiUpdateClick(ImGuiManager* imgui);
public:
	void Initialize(IDirectInput8* directInput);
	void Update();
	void ImGuiUpdate();

	//	Getter
	bool GetClick(MouseButton type);
	bool GetClickTrigger(MouseButton type);
	bool GetClickRelease(MouseButton type);
	const Vector2D& GetCursor() { return cursor_; }
	const Vector2D& GetPrevCursor() { return prevCursor_; }
	//	1フレームでのカーソルの移動量
	const Vector2D& GetMoveCursor() { return cursorMoveLen_; }
	//	ホイール回転
	int32_t GetWheel() { return click_.lZ; }

	//	Setter
	void SetShowCursor(bool showCursor) { showCursor_ = showCursor; }
	void SetLockCursor(bool lockCursor) { isLockCursor_ = lockCursor; }
};

