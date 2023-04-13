#include "ParticleManager.h"
#include "TextureManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager* instance = new ParticleManager;
	return instance;
}

void ParticleManager::DeleteInstance()
{
	delete ParticleManager::GetInstance();
}

void ParticleManager::Initialize()
{
	particleTex = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/particle.png");
}

void ParticleManager::Update()
{
#pragma region Delete
	particles.remove_if([](MoveParticle& x) {return x.GetIsEnd(); });
#pragma endregion

	for (std::forward_list<MoveParticle>::iterator it = particles.begin();
		it != particles.end();
		it++) {
		it->Update();
	}
}

void ParticleManager::MatUpdate()
{
	for (std::forward_list<MoveParticle>::iterator it = particles.begin();
		it != particles.end();
		it++) {
		it->MatUpdate();
	}
}

void ParticleManager::Draw()
{
	for (std::forward_list<MoveParticle>::iterator it = particles.begin();
		it != particles.end();
		it++) {
		it->Draw(particleTex.GetHandle());
	}
}

void ParticleManager::AddMoveParticle(const Vector3D& pos, const Vector3D& spd, int time, float scale)
{
	particles.emplace_front();
	MoveParticle& p = particles.front();
	p.SetIsBillboard(true);
	p.SetColor(Vector4D(1.0f, 1.0f, 1.0f, 0.3f));
	p.Initialize(pos, spd, scale, time);
}

