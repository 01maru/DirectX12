#include "IModel.h"

void IModel::Initialize(const char* filename, bool smoothing)
{
	LoadModel(filename, smoothing);

	for (auto& m : meshes_) {
		m->Initialzie();
	}

	for (auto& m : materials_) {
		m.second->Update();
	}

	for (auto& m : materials_) {
		m.second->LoadTexture();
	}
}

void IModel::Draw()
{
	for (auto& mesh : meshes_) {
		mesh->Draw();
	}
}

void IModel::DrawShadowReciever()
{
	for (auto& mesh : meshes_) {
		mesh->DrawShadowReciever();
	}
}
