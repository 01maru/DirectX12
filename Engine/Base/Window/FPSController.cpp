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

	//	�����l 0.0f,�T�C�Y sFPSLIST_MAX_SIZE
	fpsList_.resize(sFPSLIST_MAX_SIZE, 0.0f);
}

void FPSController::CalcFps()
{
	//	�Â������폜&���v�l�������
	totalFps_ -= fpsList_.front();
	fpsList_.pop_front();

	//	�ŐV�̏����v�Z&�ǉ�
	float nowFps = 1000000.0f / std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - reference_).count();
	fpsList_.push_back(nowFps);
	totalFps_ += nowFps;

	//	���݂̕��ϒl�v�Z
	fpsValue_ = totalFps_ / sFPSLIST_MAX_SIZE;
}

void FPSController::Update()
{
	//	1/60sec
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	//	1/65sec
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//	���݂̎��Ԏ擾
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//	�o�ߎ��Ԏ擾
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//	����������������sleep
	if (elapsed < kMinCheckTime) {
		while (std::chrono::steady_clock::now() - reference_ < kMinCheckTime) {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	CalcFps();

	//	���ݎ��Ԃ̋L�^
	reference_ = std::chrono::steady_clock::now();
}
