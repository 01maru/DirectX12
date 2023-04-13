#include "IModel.h"
#include <string>
#include <sstream>
#include <fstream>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Quaternion.h"
using namespace std;

MyDirectX* IModel::dx = MyDirectX::GetInstance();

void IModel::Initialize(const char* filename, bool smoothing)
{
	LoadModel(filename, smoothing);

	for (auto& m : meshes) {
		m->CreateBuff();
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
