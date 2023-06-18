#include "ParticleStruct.h"
#include "Easing.h"

void MoveParticle::Initialize(const Vector3D& pos, const Vector3D& velocity, float scale, const int lifeTime)
{
	SetPosition(pos);
	SetScale(3.0f);

	// Œü‚«“Ç‚Ýž‚Ý
	this->spd = velocity;
	// ‘å‚«‚³“Ç‚Ýž‚Ý
	this->maxScale = scale;

	maxTime = lifeTime;
}

void MoveParticle::Update()
{
	SetScale((float)Easing::EaseIn(3.0f, maxScale, (float)timer / (float)maxTime, 2));

	// ˆÚ“®
	Move(spd);

	// Á‚¦‚éˆ—
	if (++timer >= maxTime)
	{
		isEnd = true;
	}
}
