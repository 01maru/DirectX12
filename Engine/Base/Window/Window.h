#pragma once
#define NOMINMAX
#include <Windows.h>

class Window
{
public:
	// �E�B���h�E�T�C�Y
	static const int window_width = 1280; // ����
	static const int window_height = 720; // �c��
private:
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	// �E�B���h�E�I�u�W�F�N�g�̐���
	HWND hwnd;
	// ���b�Z�[�W
	MSG msg{};

	Window() {};
	~Window() {};
public:
	// �E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static Window* GetInstance();
	Window(const Window& obj) = delete;
	Window& operator=(const Window& obj) = delete;

	void Initialize();
	void Finalize();
	bool MsgUpdate();
	bool EndLoop();

	//	getter
	const HWND& GetHwnd() const { return hwnd; }
	const WNDCLASSEX& GetWND() const { return w;}
};

