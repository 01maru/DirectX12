#include "FrameCounter.h"
#include "FrameCounter.h"

void FrameCounter::StartCount()
{
	ResetCount();
	isActive_ = true;
}

void FrameCounter::ResetCount()
{
	if(isIncrement_)	frameCount_ = 0;
	else			frameCount_ = maxFrameCount_;
	
	count_ = 0;
}

void FrameCounter::Initialize(int maxFrameCount, bool isIncrement, bool isLoop, int maxCount)
{
	if (isIncrement_) frameCount_ = 0;
	else			  frameCount_ = maxFrameCount;

	maxFrameCount_ = maxFrameCount;
	maxCount_ = maxCount;
	isLoop_ = isLoop;
	isIncrement_ = isIncrement;

	isActive_ = false;
}

void FrameCounter::Update()
{
	if (isActive_) {

		bool countContinue = isEndless_ || count_ < maxCount_;

		if (isIncrement_)	//	�C���N�������g
		{
			frameCount_++;

			//	�͈͊O
			if (frameCount_ >= maxFrameCount_)
			{
				//	�͈͊O�ɂȂ�����C���N�������g
				count_++;

				if (countContinue) {
					//	���[�v��������f�N�������g�ɕύX
					if (isLoop_)	isIncrement_ = false;
					//	���[�v����Ȃ������珉����
					else		frameCount_ = 0;
				}
			}
		}
		else				//	�f�N�������g
		{
			frameCount_--;

			//	�͈͊O
			if (frameCount_ <= 0)
			{
				//	�͈͊O�ɂȂ�����C���N�������g
				count_++;

				if (countContinue) {
					//	���[�v��������C���N�������g�ɕύX
					if (isLoop_)	isIncrement_ = true;
					//	���[�v����Ȃ������珉����
					else		frameCount_ = maxFrameCount_;
				}
			}
		}

		//	�G���h���X����Ȃ�count���͈͊O�ɂȂ�����
		if (countContinue)	isActive_ = false;
	}
}

float FrameCounter::GetCountPerMaxCount()
{
	if (maxFrameCount_ != 0) return (float)frameCount_ / maxFrameCount_;

	//	0�Ŋ���Ȃ��悤��
	return 0.0f;
}

void FrameCounter::SetIsActive(bool isActive)
{
	if (isActive_ != isActive && isActive) ResetCount();

	isActive_ = isActive;
}

void FrameCounter::AddCount()
{
	if (frameCount_ < maxFrameCount_) frameCount_++;
}

void FrameCounter::SubCount()
{
	if (frameCount_ > 0) frameCount_--;
}
