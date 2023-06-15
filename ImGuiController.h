#pragma once

class ImGuiController
{
private:
	bool activeVolumeManager_ = false;

private:	//	�֐�
	ImGuiController() {};
	~ImGuiController() {};

public:
	static ImGuiController* GetInstance();
	ImGuiController(const ImGuiController& obj) = delete;
	ImGuiController& operator=(const ImGuiController& obj) = delete;

	void Update();

	//	Getter
	bool GetActiveVolumeManager() { return activeVolumeManager_; }
};

