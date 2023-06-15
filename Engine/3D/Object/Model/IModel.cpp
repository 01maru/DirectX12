#include "IModel.h"

void IModel::Initialize(const char* filename, bool smoothing)
{
	LoadModel(filename, smoothing);

	for (auto& m : meshes) {
		m->Initialzie();
	}

	for (auto& m : materials) {
		m.second->Update();
	}

	for (auto& m : materials) {
		m.second->LoadTexture();
	}
}

void IModel::Draw()
{
	for (auto& mesh : meshes) {
		mesh->Draw();
	}
}

void IModel::DrawShadowReciever()
{
	for (auto& mesh : meshes) {
		mesh->DrawShadowReciever();
	}
}
