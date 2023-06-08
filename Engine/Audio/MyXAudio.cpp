#include "MyXAudio.h"
#include <fstream>

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
	for (size_t i = 0; i < soundData.size(); i++)
	{
		SoundUnload(&soundData[i]);
	}
	soundData.clear();
}

int MyXAudio::SoundLoadWave(const char* filename)
{
	for (int i = 0; i < soundData.size(); i++) {
		if (soundData[i].name == filename)
		{
			//	Šù‚É‚ ‚Á‚½‚ç
			return i;
		}
	}

	std::ifstream file;
	const std::string filePath = "Resources/Sound/" + (std::string)filename;
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

	SoundData _soundData = {};

	_soundData.wfex = format.fmt;
	_soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	_soundData.bufferSize = data.size;
	_soundData.name = filename;

	soundData.push_back(_soundData);

	return soundDataIndex++;
}

void MyXAudio::SoundPlayWave(int handle_, SoundType type, float volume, bool loop)
{
	HRESULT result;

	SoundVoicePtr pSourceVoice;
	result = xAudio2->CreateSourceVoice(&pSourceVoice.ptr, &soundData[handle_].wfex, 0U, 3.0f);
	assert(SUCCEEDED(result));
	pSourceVoice.volume = volume;
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
	default:
		break;
	}
	pSourceVoice.ptr->SetVolume(volume * typeVolume);

	switch (type)
	{
	case MyXAudio::BGM:
		bgmsoundPtr.push_back(pSourceVoice);
		break;
	case MyXAudio::SE:
		soundEffectPtr.push_back(pSourceVoice);
		break;
	default:
		break;
	}

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData[handle_].pBuffer;
	buf.AudioBytes = soundData[handle_].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	//	ƒ‹[ƒvÄ¶
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
			bgmsoundPtr[i].ptr->SetVolume(bgmVolume * bgmsoundPtr[i].volume);
		}
		break;
	case MyXAudio::SE:
		seVolume = volume;
		for (size_t i = 0; i < soundEffectPtr.size(); i++)
		{
			soundEffectPtr[i].ptr->SetVolume(seVolume * soundEffectPtr[i].volume);
		}
		break;
	default:
		break;
	}
}

void MyXAudio::ChangeAllPitchRatio(float pitch)
{
	for (size_t i = 0; i < bgmsoundPtr.size(); i++)
	{
		bgmsoundPtr[i].ptr->SetFrequencyRatio(pitch);

		if (pitch <= 1.0f)	bgmsoundPtr[i].ptr->SetVolume(bgmVolume * bgmsoundPtr[i].volume);
		else				bgmsoundPtr[i].ptr->SetVolume(bgmVolume * bgmsoundPtr[i].volume * 0.5f);
	}
	for (size_t i = 0; i < soundEffectPtr.size(); i++)
	{
		soundEffectPtr[i].ptr->SetFrequencyRatio(pitch);
	}
}

//void MyXAudio::StopAllLoopSound()
//{
//	for (int i = 0; i < soundPtr.size(); i++)
//	{
//		soundPtr[i]->Stop();
//	}
//	soundPtr.resize(0);
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
	bgmsoundPtr.resize(0);
}

void MyXAudio::StopSE()
{
	for (int i = 0; i < soundEffectPtr.size(); i++)
	{
		soundEffectPtr[i].ptr->Stop();
	}
	soundEffectPtr.resize(0);
}

void MyXAudio::DeleteAllSound()
{
	for (size_t i = 0; i < soundData.size(); i++)
	{
		SoundUnload(&soundData[i]);
	}
	soundData.clear();
	soundDataIndex = 0;
}
