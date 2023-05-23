#include "JSONLoader.h"
#include <fstream>
#include <assert.h>
#include <json.hpp>

using namespace std;

LevelData* JSONLoader::LoadJSON(std::string jsonname)
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
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクト走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//	種別取得
		string type = object["type"].get<string>();
		
		//	種類ごとに処理
		//	MESHだったら
		if (type.compare("MESH") == 0) {
			levelData->objects.emplace_back(ObjectData{});

			ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//	ファイル名
				objectData.fileName = object["file_name"];
			}

			//	トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//	平行移動
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][0];
			objectData.translation.w = 1.0f;
			//	回転角
			objectData.translation.x = -(float)transform["rotation"][1];
			objectData.translation.y = -(float)transform["rotation"][2];
			objectData.translation.z = (float)transform["rotation"][0];
			objectData.translation.w = 0.0f;
			//	スケーリング
			objectData.translation.x = (float)transform["scaling"][1];
			objectData.translation.y = (float)transform["scaling"][2];
			objectData.translation.z = (float)transform["scaling"][0];
			objectData.translation.w = 0.0f;
		}

		//	再帰処理
		if (object.contains("children")) {
			
		}
	}
}
