#pragma once
#include <chrono>
#include <list>

class FPSController
{
private:
	//	List�̃T�C�Y
	static const int sFPSLIST_MAX_SIZE = 60;

	//	sFPSLIST_MAX_SIZE�t���[������fps�ۑ��p���X�g
	std::list<float> fpsList_;
	//	fpsList_�̍��v�l
	float totalFps_ = 0.0f;
	//	fpsList_�̕��ϒl(�\���p)
	float fpsValue_ = 0.0f;

	std::chrono::steady_clock::time_point reference_;

public:
	void CalcFps();

	FPSController() {};
	~FPSController() {};
public:
	static FPSController* GetInstance();
	FPSController(const FPSController& obj) = delete;
	FPSController& operator=(const FPSController& obj) = delete;

	void Initialize();
	void Update();

	float GetFPS() { return fpsValue_; }
};

