#pragma once

class ILoadingObj
{
protected:
	bool fadeIn_ = false;
	//	ロード中か
	bool loading_ = false;
	//	フェードインアウト用
	static const int sEASE_MAX_COUNT = 30;
	int easeCount_ = 0;

public:
	virtual ~ILoadingObj() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetIsLoading(bool loading);
};

