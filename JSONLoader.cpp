#include "JSONLoader.h"
#include <fstream>
#include <assert.h>

#include "ObjModel.h"

#include <map>

using namespace std;

void JSONLoader::LoadObjectData(nlohmann::json_abi_v3_11_2::detail::iter_impl<nlohmann::json_abi_v3_11_2::json>& itr, ObjectData* parent)
{
	//	種別取得
	string type = itr.value()["type"].get<string>();

	ObjectData* objectData = nullptr;
	//	種類ごとに処理
	//	MESHだったら
	if (type.compare("MESH") == 0) {
		levelData->objects.emplace_back(ObjectData{});
		objectData = &levelData->objects.back();

		if (itr.value().contains("file_name")) {
			//	ファイル名
			objectData->fileName = itr.value()["file_name"];
		}

		//	トランスフォームのパラメータ読み込み
		nlohmann::json& transform = itr.value()["transform"];
		//	平行移動
		objectData->translation.x = (float)transform["translation"][1];
		objectData->translation.y = (float)transform["translation"][2];
		objectData->translation.z = -(float)transform["translation"][0];
		//	回転角
		objectData->rotation.x = -(float)transform["rotation"][1];
		objectData->rotation.y = -(float)transform["rotation"][2];
		objectData->rotation.z = (float)transform["rotation"][0];
		//	スケーリング
		objectData->scaling.x = (float)transform["scaling"][1];
		objectData->scaling.y = (float)transform["scaling"][2];
		objectData->scaling.z = (float)transform["scaling"][0];

		objectData->parent = parent;
	}
	
	//	再帰処理
	if (itr.value().contains("children")) {
		for (auto itr_cild = itr.value()["children"].begin(); itr_cild < itr.value()["children"].end(); ++itr_cild)
		{
			LoadObjectData(itr_cild, objectData);
		}
	}
}

void JSONLoader::LoadJSON(std::string jsonname)
{
	std::ifstream file;
	const string filename = jsonname + ".json";
	const string directoryPath = "Resources/Levels/";

	//	ファイルを開く
	file.open(directoryPath + filename);

	//	失敗したら
	if (file.fail()) assert(0);

	nlohmann::json deserialized;
	//	JSON文字列から解凍
	file >> deserialized;

	//	正しいデータかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//	"name"を文字列取得
	string name = deserialized["name"].get<string>();
	//	正しいデータかチェック
	assert(name.compare("scene") == 0);

	//	格納用
	levelData = new LevelData();

	// "objects"の全オブジェクト走査
	for (auto itr = deserialized["objects"].begin(); itr < deserialized["objects"].end(); ++itr)
	{
		assert(itr.value().contains("type"));

		LoadObjectData(itr, nullptr);
	}

	LoadModel();

	for (auto& objectData : levelData->objects)
	{
		IModel* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);

		if (it != models.end()) { model = it->second; }

		Object3D* newObject = Object3D::Create(model);
		
		newObject->SetPosition(objectData.translation);
		newObject->SetRotation(objectData.rotation);
		newObject->SetScale(objectData.scaling);

		objects.push_back(newObject);
	}
}

void JSONLoader::LoadModel()
{
	for (auto& objectData : levelData->objects) {
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it == models.end()) {
			ObjModel* model = new ObjModel(objectData.fileName.c_str());
			models[objectData.fileName] = model;
		}
	}
}

void JSONLoader::MatUpdate()
{
	for (Object3D* object : objects) {
		object->MatUpdate();
	}
}

void JSONLoader::Draw()
{
	for (Object3D* object : objects) {
		object->Draw();
	}
}
