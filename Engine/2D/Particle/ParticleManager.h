#pragma once
#include <forward_list>
#include "Texture.h"

class ParticleManager
{
private:
	//std::forward_list<MoveParticle> particles;

#pragma region Texture
	Texture particleTex_;
#pragma endregion

	ParticleManager() {};
	~ParticleManager() {};
public:
	static ParticleManager* GetInstance();
	ParticleManager(const ParticleManager& obj) = delete;
	ParticleManager& operator=(const ParticleManager& obj) = delete;

	void Initialize();
	void Update();
	void MatUpdate();
	void Draw();

#pragma region ADD

	//void AddMoveParticle(const Vector3D& pos, const Vector3D& spd, int time, float scale = 3.0f);

#pragma endregion
};

