#pragma once

class Framework
{
protected:
	//	ゲームループ用フラグ
	bool gameroopFlag = true;
public:
	virtual ~Framework() = default;

	void Run();

	virtual void Initialize();
	virtual void Update();
	virtual void Draw() = 0;
	virtual void Finalize();
};

