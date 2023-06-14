#pragma once
#include <chrono>

class FPS
{
private:
	std::chrono::steady_clock::time_point reference_;

	FPS() {};
	~FPS() {};
public:
	static FPS* GetInstance();
	FPS(const FPS& obj) = delete;
	FPS& operator=(const FPS& obj) = delete;

	void Initialize();
	void Update();
};

