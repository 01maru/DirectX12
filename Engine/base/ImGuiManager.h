#pragma once
#include <wrl.h>
#include <d3d12.h>

class ImGuiManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12DescriptorHeap> srvHeap;

	ImGuiManager() {};
	~ImGuiManager() {};
public:
	static ImGuiManager* GetInstance();
	//	�R�s�[�R���X�g���N�^����
	ImGuiManager(const ImGuiManager& obj) = delete;
	//	������Z�q����
	ImGuiManager& operator=(const ImGuiManager& obj) = delete;

	void Initialize();
	void Finalize();
	
	//	�X�V�O����
	void Begin();
	//	�X�V�㏈��
	void End();
	//	�`�揈��
	void Draw();
};

