#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>

#include "Vector2D.h"

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

	void BeginWindow(const std::string& windowName);
	void BeginWindow(const std::string& windowName, bool menu);
	void EndWindow();

	bool BeginMenuBar();
	void EndMenuBar();
	bool BeginMenu(const std::string& menuName);
	void EndMenu();
	bool MenuItem(const std::string& name);
	void SameLine();
	bool SetButton(const std::string& buttonName, const Vector2D& size = Vector2D());
	void SetRadioButton(const std::string& buttonName, bool& flag);
	void SetSliderFloat(const std::string& sliderName, float& value, float spd, float minValue, float maxValue);
	void CheckBox(const std::string& name, bool& flag);

	void BeginChild(const Vector2D& size = Vector2D());
	void EndChild();
};

