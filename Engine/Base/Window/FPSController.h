#pragma once
#include <chrono>
#include <list>

class FPSController
{
private:
	//	Listのサイズ
	static const int sFPSLIST_MAX_SIZE = 60;

	//	sFPSLIST_MAX_SIZEフレーム分のfps保存用リスト
	std::list<float> fpsList_;
	//	fpsList_の合計値
	float totalFps_ = 0.0f;
	//	fpsList_の平均値(表示用)
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

