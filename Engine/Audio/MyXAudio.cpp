#include "MyXAudio.h"
#include <fstream>

#include "ImGuiManager.h"
#include "Input.h"
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

void MyXAudio::SoundUnload(SoundData* soundData_)
{
	delete[] soundData_->pBuffer;

	soundData_->pBuffer = 0;
	soundData_->bufferSize = 0;
	soundData_->wfex = {};
}

MyXAudio* MyXAudio::GetInstance()
{
	static MyXAudio instance;
	return &instance;
}

void MyXAudio::Initialize()
{
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

void MyXAudio::Finalize()
{
	StopAllSound();
	masterVoice->DestroyVoice();
	xAudio2.Reset();

	for (auto itr = data_.begin(); itr != data_.end(); ++itr)
	{
		SoundUnload(&itr->second);
	}
	data_.clear();
}

void MyXAudio::Update()
{
#ifdef _DEBUG

	if (!isDebug_) return;

	for (size_t i = 0; i < bgmsoundPtr.size(); i++)
	{
		bgmsoundPtr[i].ptr->SetVolume(bgmVolume * data_[bgmsoundPtr[i].soundname].volume);
	}
	for (size_t i = 0; i < soundEffectPtr.size(); i++)
	{
		soundEffectPtr[i].ptr->SetVolume(seVolume * data_[soundEffectPtr[i].soundname].volume);
	}

#endif // _DEBUG
}

void MyXAudio::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->SetWindow("Volume");

	imguiMan->CheckBox("IsDebug", isDebug_);
	
	if (isDebug_) {
		for (auto itr = data_.begin(); itr != data_.end(); ++itr)
		{
			imguiMan->SetSliderFloat(itr->first.c_str(), itr->second.volume, 0.0f, 1.0f);
		}

		imguiMan->SetButton("Save");
	}

	imguiMan->EndWindow();
}

void MyXAudio::LoadSoundWave(const std::string& filename)
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

	//	データの挿入
	data_.emplace(filename, soundData);
}

void MyXAudio::PlaySoundWave(const std::string& soundName, SoundType type, bool loop)
{
	HRESULT result;
	
	//	音データがあったら
	assert(data_.count(soundName) != 0);

	SoundVoicePtr pSourceVoice;
	result = xAudio2->CreateSourceVoice(&pSourceVoice.ptr, &data_[soundName].wfex, 0U, 3.0f);

	pSourceVoice.soundname = soundName;
	float typeVolume = 0.0f;
	switch (type)
	{
	case MyXAudio::Master:
		typeVolume = masterVolume;
		break;
	case MyXAudio::BGM:
		typeVolume = bgmVolume;
		break;
	case MyXAudio::SE:
		typeVolume = seVolume;
		break;
	}
	pSourceVoice.ptr->SetVolume(data_[soundName].volume * typeVolume);

	//	配列に挿入
	switch (type)
	{
	case MyXAudio::BGM:
		bgmsoundPtr.push_back(pSourceVoice);
		break;
	case MyXAudio::SE:
		soundEffectPtr.push_back(pSourceVoice);
		break;
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

void MyXAudio::VolumeUpdate(SoundType type)
{
	Input* input = Input::GetInstance();

	float volume = ((input->GetKey(DIK_D) || input->GetKey(DIK_RIGHT)) - (input->GetKey(DIK_A) || input->GetKey(DIK_LEFT))) * 0.01f;

	switch (type)
	{
	case MyXAudio::Master:
		volume += masterVolume;
		break;
	case MyXAudio::BGM:
		volume += bgmVolume;
		break;
	case MyXAudio::SE:
		volume += seVolume;
		break;
	default:
		break;
	}
	volume = MyMath::mMax(0.0f, volume);
	volume = MyMath::mMin(1.0f, volume);

	ChangeVolume(volume, type);
}

void MyXAudio::ChangeVolume(float volume, SoundType type)
{
	switch (type)
	{
	case MyXAudio::Master:
		masterVolume = volume;
		masterVoice->SetVolume(masterVolume);
		break;
	case MyXAudio::BGM:
		bgmVolume = volume;
		for (size_t i = 0; i < bgmsoundPtr.size(); i++)
		{
			bgmsoundPtr[i].ptr->SetVolume(bgmVolume * data_[bgmsoundPtr[i].soundname].volume);
		}
		break;
	case MyXAudio::SE:
		seVolume = volume;
		for (size_t i = 0; i < soundEffectPtr.size(); i++)
		{
			soundEffectPtr[i].ptr->SetVolume(seVolume * data_[soundEffectPtr[i].soundname].volume);
		}
		break;
	default:
		break;
	}
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

void MyXAudio::StopAllSound()
{
	StopBGM();
	StopSE();
}

void MyXAudio::StopBGM()
{
	for (int i = 0; i < bgmsoundPtr.size(); i++)
	{
		bgmsoundPtr[i].ptr->Stop();
	}
	bgmsoundPtr.clear();
}

void MyXAudio::StopSE()
{
	for (int i = 0; i < soundEffectPtr.size(); i++)
	{
		soundEffectPtr[i].ptr->Stop();
	}
	soundEffectPtr.clear();
}

void MyXAudio::DeleteAllSound()
{
	for (auto itr = data_.begin(); itr != data_.end(); ++itr)
	{
		SoundUnload(&itr->second);
	}
	data_.clear();
}
