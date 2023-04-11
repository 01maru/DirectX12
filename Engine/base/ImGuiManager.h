#pragma once
#include "DirectX.h"

class ImGuiManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12DescriptorHeap> srvHeap;
public:
	ImGuiManager() {};
	~ImGuiManager() {};

	static ImGuiManager* GetInstance();
	static void DeleteInstance();
	//	�R�s�[�R���X�g���N�^����
	ImGuiManager(const ImGuiManager& obj) = delete;
	//	������Z�q����
	ImGuiManager& operator=(const ImGuiManager& obj) = delete;

	void Initialize();
	
	//	�X�V�O����
	void Begin();
	//	�X�V�㏈��
	void End();
	//	�`�揈��
	void Draw();
};

