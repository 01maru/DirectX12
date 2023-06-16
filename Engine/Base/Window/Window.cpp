#include "Window.h"
#include <imgui_impl_win32.h>

#pragma comment(lib,"winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) return true;
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window* Window::GetInstance()
{
	static Window instance;
	return &instance;
}

void Window::Initialize()
{
	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)WindowProc;		// ウィンドウプロシージャを設定
	w_.lpszClassName = L"DirectXGame";			// ウィンドウクラス名
	w_.hInstance = GetModuleHandle(nullptr);	// ウィンドウハンドル
	w_.hCursor = LoadCursor(NULL, IDC_ARROW);	// カーソル指定

	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w_);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, sWIN_WIDTH, sWIN_HEIGHT };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(w_.lpszClassName,	// クラス名
		L"DirectXGame",						// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,				// 標準的なウィンドウスタイル
		CW_USEDEFAULT,						// 表示X座標（OSに任せる）
		CW_USEDEFAULT,						// 表示Y座標（OSに任せる）
		wrc.right - wrc.left,				// ウィンドウ横幅
		wrc.bottom - wrc.top,				// ウィンドウ縦幅
		nullptr,							// 親ウィンドウハンドル
		nullptr,							// メニューハンドル
		w_.hInstance,						// 呼び出しアプリケーションハンドル
		nullptr);							// オプション
	// ウィンドウを表示状態にする
	ShowWindow(hwnd_, SW_SHOW);

	timeBeginPeriod(1);
}

void Window::Finalize()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w_.lpszClassName, w_.hInstance);
}

bool Window::MsgUpdate()
{
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_);	// キー入力メッセージの処理
		DispatchMessage(&msg_);		// プロシージャにメッセージを送る
	}

	return EndLoop();
}

bool Window::EndLoop()
{
	if (msg_.message == WM_QUIT) return true;
	return false;
}
