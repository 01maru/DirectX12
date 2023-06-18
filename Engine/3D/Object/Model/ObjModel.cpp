#include "ObjModel.h"
#include <sstream>
#include <fstream>
#include <assert.h>

using namespace std;

ObjModel::ObjModel(const char* filename, bool smoothing)
{
	Initialize(filename, smoothing);
}

ObjModel::~ObjModel()
{
	meshes_.clear();

	materials_.clear();
}

void ObjModel::BoneTransform(float /*TimeInSeconds*/, std::vector<Matrix>& /*transforms*/)
{
	//	objなのでboneなし
}

void ObjModel::AddIndices(const std::vector<uint16_t>& indices, Mesh* mesh)
{
	if (indices.size() == 3)
	{
		mesh->AddIndex(indices[0]);
		mesh->AddIndex(indices[2]);
		mesh->AddIndex(indices[1]);
	}
	else {
		mesh->AddIndex(indices[0]);
		mesh->AddIndex(indices[2]);
		mesh->AddIndex(indices[1]);
		mesh->AddIndex(indices[3]);
		mesh->AddIndex(indices[5]);
		mesh->AddIndex(indices[4]);
	}
}

void ObjModel::LoadModel(const std::string& modelname, bool smoothing)
{
	std::vector<unsigned short> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3D> temp_poss;
	std::vector<Vector2D> temp_uvs;
	std::vector<Vector3D> temp_normals;

	//	filePath
	const string directoryPath = "Resources/Model/" + modelname + "/";
	const string filename = modelname + ".obj";

	//	FileOpen
	std::ifstream file_;
	file_.open(directoryPath + filename);
	assert(!file_.fail());

	meshes_.emplace_back(new Mesh);
	Mesh* mesh = meshes_.back().get();
	int indexCount = 0;

	string line;
	while (getline(file_, line)) {
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		//	material
		if (key == "mtllib") {
			string filename_;
			line_stream >> filename_;
			LoadMaterial(directoryPath, filename_);
		}
		
		//	materialの割り当て
		if (key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr) {
				// マテリアルの名読み込み
				string materialName;
				line_stream >> materialName;

				// マテリアル名で検索し、マテリアルを割り当てる
				auto itr = materials_.find(materialName);
				if (itr != materials_.end()) {
					mesh->SetMaterial(itr->second.get());
				}
			}
		}

		if (key == "o") {

			// メッシュの情報が揃っているなら
			if (mesh->GetVertexCount() > 0) {
				// 頂点法線の平均によるエッジの平滑化
				if (smoothing) {
					mesh->CalcSmoothedNormals();
				}
				// 次のメッシュ生成
				meshes_.emplace_back(new Mesh);
				mesh = meshes_.back().get();
				indexCount = 0;
			}

			//// グループ名読み込み
			//string groupName;
			//line_stream >> groupName;

			//// メッシュに名前をセット
			//mesh->SetName(groupName);
		}

		//	Vertex
		if (key == "v") {
			Vector3D pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
			pos.z = -pos.z;

			temp_poss.emplace_back(pos);
		}

		//	UV
		if (key == "vt") {
			Vector2D uv;
			line_stream >> uv.x;
			line_stream >> uv.y;
			uv.y = 1.0f - uv.y;

			temp_uvs.emplace_back(uv);
		}

		//	Normal
		if (key == "vn") {
			Vector3D normal;
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			temp_normals.emplace_back(normal);
		}

		//	face
		if (key == "f") {
			int indexNum = 0;
			std::vector<uint16_t> indices;

			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//	頂点ごとのIndex情報取得
				std::istringstream index_stream(index_string);
				unsigned short indexPos, indexNormal, indexUV;
				index_stream >> indexPos;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexUV;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				//	AddVertex
				ModelVertex vertex{};
				vertex.pos = temp_poss[indexPos - 1];
				vertex.normal = temp_normals[indexNormal - 1];
				vertex.uv = temp_uvs[indexUV - 1];
				vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
				mesh->AddVertex(vertex);

				if (smoothing) {
					mesh->AddSmoothData(indexPos, (unsigned short)mesh->GetVertexCount() - 1);
				}

				if (indexNum >= 3)	//	一面のindex数が3を越えたら(4つ以上は想定外)
				{
					indices.emplace_back((unsigned short)(indexCount - 1));
					indices.emplace_back((unsigned short)indexCount);
					indices.emplace_back((unsigned short)(indexCount - 3));
				}
				else
				{
					indices.emplace_back((unsigned short)indexCount);
				}
				indexNum++;
				indexCount++;
			}

			//	AddIndex
			AddIndices(indices, mesh);
		}
	}

	file_.close();

	if (smoothing) {
		mesh->CalcSmoothedNormals();
	}
}

void ObjModel::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//	fileOpen
	std::ifstream file;
	file.open(directoryPath + filename);
	if (file.fail()) { assert(0); }

	//	複数マテリアルに対応するために初期値nullptr
	Material* material = nullptr;
	string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		if (key == "newmtl") {
			// 既にマテリアルがあれば
			if (material) {
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = Material::Create();
			// マテリアル名読み込み
			if (material) {
				line_stream >> material->name_;
			}
		}

		if (material)
		{
			if (key == "Ka") {
				line_stream >> material->ambient_.x;
				line_stream >> material->ambient_.y;
				line_stream >> material->ambient_.z;
			}

			if (key == "Kd") {
				line_stream >> material->diffuse_.x;
				line_stream >> material->diffuse_.y;
				line_stream >> material->diffuse_.z;
			}

			if (key == "Ks") {
				line_stream >> material->specular_.x;
				line_stream >> material->specular_.y;
				line_stream >> material->specular_.z;
			}

			//	画像セット
			if (key == "map_Kd") {
				line_stream >> material->textureFilename_;

				string filepath = directoryPath + material->textureFilename_;
				MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, material->wfilepath_, _countof(material->wfilepath_));
			}
		}
	}
	file.close();

	if (material) {
		// マテリアルを登録
		AddMaterial(material);
	}
}
