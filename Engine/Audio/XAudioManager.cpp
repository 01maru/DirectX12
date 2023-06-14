#include "XAudioManager.h"
#include <fstream>
#include <assert.h>
#include <sstream>

#include "ImGuiManager.h"
#include "MyMath.h"

#pragma region Struct

struct ChunkHeader {
	char id[4];
	int32_t size;
};
struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};
struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

#pragma endregion

void XAudioManager::UnloadSoundData(SoundData* soundData)
{
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

XAudioManager* XAudioManager::GetInstance()
{
	static XAudioManager instance;
	return &instance;
}

void XAudioManager::Initialize()
{
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

void XAudioManager::Finalize()
{
	StopAllSound();
	masterVoice->DestroyVoice();
	xAudio2.Reset();

	for (auto itr = data_.begin(); itr != data_.end(); ++itr)
	{
		UnloadSoundData(&itr->second);
	}
	data_.clear();
}

float XAudioManager::LoadVolume(const std::string& filename)
{
	float volume = 1.0f;

	std::string filePath = "Resources/Sound/VolumeData.txt";

	//ファイル開く(開けなかったら新規作成)
	std::ifstream file;
	file.open(filePath.c_str(), std::ios_base::app);

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		//[//]から始まる行はコメント
		if (key.find("//") == 0) continue;

		if (key == filename) {
			//	dataがあったら
			line_stream >> volume;
		}
	}

	//ファイル閉じる
	file.close();

	return volume;
}

void XAudioManager::LoadAllValumeData()
{
	for (auto itr = data_.begin(); itr != data_.end(); ++itr)
	{
		itr->second.volume = LoadVolume(itr->first);
	}
}

void XAudioManager::SaveVolume()
{
}

void XAudioManager::Update()
{
#ifdef _DEBUG

	if (!isDebug_) return;

	for (size_t i = 0; i < bgmsoundPtr.size(); i++)
	{
		bgmsoundPtr[i].ptr->SetVolume(bgmVolume * data_[bgmsoundPtr[i].dataKey].volume);
	}
	for (size_t i = 0; i < soundEffectPtr.size(); i++)
	{
		soundEffectPtr[i].ptr->SetVolume(seVolume * data_[soundEffectPtr[i].dataKey].volume);
	}

#endif // _DEBUG
}

void XAudioManager::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("VolumeManager", true);

	if (imguiMan->BeginMenuBar()) {
		if (imguiMan->BeginMenu("File")) {
			if (imguiMan->MenuItem("Save")) SaveVolume();
			if (imguiMan->MenuItem("Load")) LoadAllValumeData();
			imguiMan->EndMenu();
		}
		imguiMan->EndMenuBar();
	}

	imguiMan->CheckBox("IsDebug", isDebug_);

	if (isDebug_) {
		imguiMan->BeginChild(Vector2D(0, 200));

		for (auto itr = data_.begin(); itr != data_.end(); ++itr)
		{
			imguiMan->SetSliderFloat(itr->first.c_str(), itr->second.volume, 0.005f, 0.0f, 1.0f);

			if (imguiMan->SetButton("Play	"+ itr->first)) {
				PlayDebugSoundWave(itr->first, Master, false, true);
			}
		}
		imguiMan->EndChild();

		if (imguiMan->SetButton("StopAllSound")) StopDebugSound();
	}

	imguiMan->EndWindow();
}

void XAudioManager::LoadSoundWave(const std::string& filename)
{
	//	既に読み込み済みだったら
	if (data_.count(filename) == 1) return;

	std::ifstream file;
	const std::string filePath = "Resources/Sound/" + filename;
	file.open(filePath, std::ios_base::binary);
	assert(file.is_open());

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	FormatChunk format = {};

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) { assert(0); }
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK", 4) == 0) {
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) { assert(0); }
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	file.close();

	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
	soundData.volume = LoadVolume(filename);

	//	データの挿入
	data_.emplace(filename, soundData);
}

void XAudioManager::PlayDebugSoundWave(const std::string& soundName, SoundType type, bool loop, bool isDebug)
{
	HRESULT result;

	//	音データがあったら
	assert(data_.count(soundName) != 0);

	SoundVoicePtr pSourceVoice;
	result = xAudio2->CreateSourceVoice(&pSourceVoice.ptr, &data_[soundName].wfex, 0U, 3.0f);

	pSourceVoice.dataKey = soundName;
	float typeVolume = 1.0f;
	switch (type)
	{
	case XAudioManager::BGM:
		typeVolume = bgmVolume;
		break;
	case XAudioManager::SE:
		typeVolume = seVolume;
		break;
	}
	pSourceVoice.ptr->SetVolume(data_[soundName].volume * typeVolume);

	//	配列に挿入
	if (isDebug) {
		debugSoundPtr_.push_back(pSourceVoice);
	}
	else {
		switch (type)
		{
		case XAudioManager::BGM:
			bgmsoundPtr.push_back(pSourceVoice);
			break;
		case XAudioManager::SE:
			soundEffectPtr.push_back(pSourceVoice);
			break;
		}
	}

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = data_[soundName].pBuffer;
	buf.AudioBytes = data_[soundName].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	//	ループ再生
	if (loop) buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	result = pSourceVoice.ptr->SubmitSourceBuffer(&buf);
	result = pSourceVoice.ptr->Start();
}

void XAudioManager::PlaySoundWave(const std::string& soundName, SoundType type, bool loop)
{
	PlayDebugSoundWave(soundName, type, loop);
}

void XAudioManager::ChangeVolume(float volume, SoundType type)
{
	switch (type)
	{
	case XAudioManager::Master:
		masterVolume = volume;
		masterVoice->SetVolume(masterVolume);
		break;
	case XAudioManager::BGM:
		bgmVolume = volume;
		for (size_t i = 0; i < bgmsoundPtr.size(); i++)
		{
			bgmsoundPtr[i].ptr->SetVolume(bgmVolume * data_[bgmsoundPtr[i].dataKey].volume);
		}
		break;
	case XAudioManager::SE:
		seVolume = volume;
		for (size_t i = 0; i < soundEffectPtr.size(); i++)
		{
			soundEffectPtr[i].ptr->SetVolume(seVolume * data_[soundEffectPtr[i].dataKey].volume);
		}
		break;
	default:
		break;
	}
}

void XAudioManager::VolumeUpdate(SoundType type, int inputValue)
{
	float volume = inputValue * 0.01f;

	switch (type)
	{
	case XAudioManager::Master:
		volume += masterVolume;
		break;
	case XAudioManager::BGM:
		volume += bgmVolume;
		break;
	case XAudioManager::SE:
		volume += seVolume;
		break;
	default:
		break;
	}
	volume = MyMath::mMax(0.0f, volume);
	volume = MyMath::mMin(1.0f, volume);

	ChangeVolume(volume, type);
}

//void MyXAudio::ChangeAllPitchRatio(float pitch)
//{
//	for (size_t i = 0; i < bgmsoundPtr.size(); i++)
//	{
//		bgmsoundPtr[i].ptr->SetFrequencyRatio(pitch);
//
//		if (pitch <= 1.0f)	bgmsoundPtr[i].ptr->SetVolume(bgmVolume * bgmsoundPtr[i].volume);
//		else				bgmsoundPtr[i].ptr->SetVolume(bgmVolume * bgmsoundPtr[i].volume * 0.5f);
//	}
//	for (size_t i = 0; i < soundEffectPtr.size(); i++)
//	{
//		soundEffectPtr[i].ptr->SetFrequencyRatio(pitch);
//	}
//}

void XAudioManager::StopAllSound()
{
	StopBGM();
	StopSE();
	StopDebugSound();
}

void XAudioManager::StopBGM()
{
	for (int i = 0; i < bgmsoundPtr.size(); i++)
	{
		bgmsoundPtr[i].ptr->Stop();
	}
	bgmsoundPtr.clear();
}

void XAudioManager::StopSE()
{
	for (int i = 0; i < soundEffectPtr.size(); i++)
	{
		soundEffectPtr[i].ptr->Stop();
	}
	soundEffectPtr.clear();
}

void XAudioManager::StopSound(const std::string& soundName)
{
	for (int i = 0; i < debugSoundPtr_.size(); i++)
	{
		if (debugSoundPtr_[i].dataKey != soundName) continue;
		debugSoundPtr_[i].ptr->Stop();
	}
}

void XAudioManager::StopDebugSound()
{
	for (int i = 0; i < debugSoundPtr_.size(); i++)
	{
		debugSoundPtr_[i].ptr->Stop();
	}
	debugSoundPtr_.clear();
}

void XAudioManager::DeleteAllSound()
{
	for (auto& data : data_)
	{
		UnloadSoundData(&data.second);
	}
	data_.clear();
}

void XAudioManager::DeleteSoundData(const std::string& soundName)
{
	//	音データがあったら
	assert(data_.count(soundName) != 0);
	
	UnloadSoundData(&data_[soundName]);
	data_.erase(soundName);
}
