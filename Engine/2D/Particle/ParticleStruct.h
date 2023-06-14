#pragma once
#include "Vector3D.h"
#include "Particle.h"

class MoveParticle :public Particle
{
public:
	void Initialize(const Vector3D& pos,
		const Vector3D& velocity,
		float scale_,
		const int lifeTime);
	void Update();

	bool GetIsEnd() { return isEnd; }
private:
	Vector3D spd;	//	速度

	float maxScale = 0;	//	最大スケール
	int timer = 0;		//	今の時間
	int maxTime = 0;	//	生成時間
	bool isEnd = false;
};

