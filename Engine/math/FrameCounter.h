#pragma once
#include <cstdint>

class FrameCounter
{
private:
	//	現在の経過フレーム数
	int32_t frameCount_ = 0;
	//	フレームの範囲(0~maxFrameCount)
	int32_t maxFrameCount_ = 0;

	//	範囲外に言った回数
	int32_t count_ = 0;
	//	終了までのnowCountの回数
	int32_t maxCount_ = 0;

	bool isEndless_ = false;
	bool isLoop_ = true;
	bool isIncrement_ = true;
	bool isActive_ = false;

public:
	FrameCounter() {};

	void StartCount();
	void ResetCount();

	void Initialize(int32_t maxFrameCount, bool isIncrement, bool isLoop = false, int32_t maxCount = 1);
	void Update();

	//	Getter
	bool GetIsActive()			{ return isActive_; }
	bool GetIsIncrement()		{ return isIncrement_; }
	int32_t GetCount()			{ return count_; }
	int32_t GetMaxCount()		{ return maxCount_; }
	int32_t GetFrameCount()		{ return frameCount_; }
	int32_t GetMaxFrameCount()	{ return maxFrameCount_; }
	//	0~1.0fの間の値を返す
	float GetCountPerMaxCount();

	//	Setter
	void SetIsActive(bool isActive);
	void SetIsIncrement(bool isIncrement) { isIncrement_ = isIncrement; }
	void AddCount();
	void SubCount();
	void SetIsEndless(bool isEndless) { isEndless_ = isEndless; }
};

