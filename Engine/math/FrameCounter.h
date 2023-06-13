#pragma once

class FrameCounter
{
private:
	//	現在の経過フレーム数
	int frameCount_ = 0;
	//	フレームの範囲(0~maxFrameCount)
	int maxFrameCount_ = 0;

	//	範囲外に言った回数
	int count_ = 0;
	//	終了までのnowCountの回数
	int maxCount_ = 0;

	bool isEndless_ = false;
	bool isLoop_ = true;
	bool isIncrement_ = true;
	bool isActive_ = false;

public:
	FrameCounter() {};

	void StartCount();
	void ResetCount();

	void Initialize(int maxFrameCount, bool isIncrement, bool isLoop = false, int maxCount = 1);
	void Update();

	//	Getter
	bool GetIsActive()		{ return isActive_; }
	bool GetIsIncrement()	{ return isIncrement_; }
	int GetCount()			{ return count_; }
	int GetMaxCount()		{ return maxCount_; }
	int GetFrameCount()		{ return frameCount_; }
	int GetMaxFrameCount()	{ return maxFrameCount_; }
	//	0~1.0fの間の値を返す
	float GetCountPerMaxCount();

	//	Setter
	void SetIsActive(bool isActive);
	void SetIsIncrement(bool isIncrement) { isIncrement_ = isIncrement; }
	void AddCount();
	void SubCount();
	void SetIsEndless(bool isEndless) { isEndless_ = isEndless; }
};

