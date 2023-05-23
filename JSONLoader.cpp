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

	//	�t�@�C�����J��
	file.open(directoryPath + filename);

	//	���s������
	if (file.fail()) assert(0);

	nlohmann::json deserialized;
	//	JSON�����񂩂��
	file >> deserialized;

	//	�������f�[�^���`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//	"name"�𕶎���擾
	string name = deserialized["name"].get<string>();
	//	�������f�[�^���`�F�b�N
	assert(name.compare("scene") == 0);

	//	�i�[�p
	LevelData* levelData = new LevelData();

	// "objects"�̑S�I�u�W�F�N�g����
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//	��ʎ擾
		string type = object["type"].get<string>();
		
		//	��ނ��Ƃɏ���
		//	MESH��������
		if (type.compare("MESH") == 0) {
			levelData->objects.emplace_back(ObjectData{});

			ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//	�t�@�C����
				objectData.fileName = object["file_name"];
			}

			//	�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			//	���s�ړ�
			objectData.translation.x = (float)transform["translation"][1];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = -(float)transform["translation"][0];
			objectData.translation.w = 1.0f;
			//	��]�p
			objectData.translation.x = -(float)transform["rotation"][1];
			objectData.translation.y = -(float)transform["rotation"][2];
			objectData.translation.z = (float)transform["rotation"][0];
			objectData.translation.w = 0.0f;
			//	�X�P�[�����O
			objectData.translation.x = (float)transform["scaling"][1];
			objectData.translation.y = (float)transform["scaling"][2];
			objectData.translation.z = (float)transform["scaling"][0];
			objectData.translation.w = 0.0f;
		}

		//	�ċA����
		if (object.contains("children")) {
			
		}
	}
}
