#include "FPSController.h"
#include <thread>

FPSController* FPSController::GetInstance()
{
	static FPSController instance;
	return &instance;
}

void FPSController::Initialize()
{
	reference_ = std::chrono::steady_clock::now();

	//	初期値 0.0f,サイズ sFPSLIST_MAX_SIZE
	fpsList_.resize(sFPSLIST_MAX_SIZE, 0.0f);
}

void FPSController::CalcFps()
{
	//	古い情報を削除&合計値から引く
	totalFps_ -= fpsList_.front();
	fpsList_.pop_front();

	//	最新の情報を計算&追加
	float nowFps = 1000000.0f / std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - reference_).count();
	fpsList_.push_back(nowFps);
	totalFps_ += nowFps;

	//	現在の平均値計算
	fpsValue_ = totalFps_ / sFPSLIST_MAX_SIZE;
}

void FPSController::Update()
{
	//	1/60sec
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	//	1/65sec
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//	現在の時間取得
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//	経過時間取得
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//	処理が速かったらsleep
	if (elapsed < kMinCheckTime) {
		while (std::chrono::steady_clock::now() - reference_ < kMinCheckTime) {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	CalcFps();

	//	現在時間の記録
	reference_ = std::chrono::steady_clock::now();
}
