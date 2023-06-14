#pragma once

class ILoadingObj
{
protected:
	bool fadeIn_ = false;
	//	���[�h����
	bool loading_ = false;
	//	�t�F�[�h�C���A�E�g�p
	static const int sEaseMaxCount = 30;
	int easeCount_ = 0;

public:
	virtual ~ILoadingObj() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetIsLoading(bool loading);
};

