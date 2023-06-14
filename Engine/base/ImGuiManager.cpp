#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include "Window.h"
#include "DirectX.h"

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(Window::GetInstance()->GetHwnd());

	MyDirectX* dx = MyDirectX::GetInstance();
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT result = dx->GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
	ImGui_ImplDX12_Init(dx->GetDev(), 2,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap.Get(),
		srvHeap->GetCPUDescriptorHandleForHeapStart(),
		srvHeap->GetGPUDescriptorHandleForHeapStart());

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

void ImGuiManager::SetWindow(const std::string& windowName)
{
	ImGui::Begin(windowName.c_str());
}

void ImGuiManager::EndWindow()
{
	ImGui::End();
}

bool ImGuiManager::SetButton(const std::string& buttonName, const Vector2D& size)
{
	return ImGui::Button(buttonName.c_str(), ImVec2(size.x, size.y));
}

void ImGuiManager::SetRadioButton(const std::string& buttonName, bool& flag)
{
	flag = ImGui::RadioButton(buttonName.c_str(), !flag);
}

void ImGuiManager::SetSliderFloat(const std::string& sliderName, float& value, float spd, float minValue, float maxValue)
{
	//ImGui::SliderFloat(sliderName.c_str(), &value, minValue, maxValue);
	ImGui::DragFloat(sliderName.c_str(), &value, spd, minValue, maxValue);
}

void ImGuiManager::CheckBox(const std::string& name, bool& flag)
{
	ImGui::Checkbox(name.c_str(), &flag);
}

void ImGuiManager::BeginChild(const Vector2D& size)
{
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(size.x, size.y), ImGuiWindowFlags_NoTitleBar);
}

void ImGuiManager::EndChild()
{
	ImGui::EndChild();
}
