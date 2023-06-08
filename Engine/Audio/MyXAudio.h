#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <vector>
#include <string>

#pragma comment(lib,"xaudio2.lib")

struct SoundData {
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	unsigned int bufferSize;
	std::string name;
};

struct SoundVoicePtr {
	IXAudio2SourceVoice* ptr;
	float volume = 1.0f;
};

class MyXAudio
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;

	//	soundのデータ配列
	std::vector<SoundData> soundData;
	int soundDataIndex = 0;

	std::vector<SoundVoicePtr> soundEffectPtr;
	std::vector<SoundVoicePtr> bgmsoundPtr;

	//	volume
	float bgmVolume = 1.0f;
	float seVolume = 1.0f;
	float masterVolume = 1.0f;

	float pitchRatio = 1.0f;

public:
	//	音量調節用Enum
	enum SoundType {
		Master,
		BGM,
		SE,
	};

private:
	void SoundUnload(SoundData* soundData_);

	MyXAudio() {};
	~MyXAudio() {};
public:
	static MyXAudio* GetInstance();
	MyXAudio(const MyXAudio& obj) = delete;
	MyXAudio& operator=(const MyXAudio& obj) = delete;

	void Initialize();
	void Finalize();
	//	load
	int SoundLoadWave(const char* filename);
	//	volume変更用
	void VolumeUpdate(SoundType type);
	void ChangeVolume(float volume, SoundType type);
	void ChangeAllPitchRatio(float pitch);

	//	play
	//	volumeは0.0f～1.0f
	void SoundPlayWave(int handle_, SoundType type, float volume = 1.0f, bool loop = false);

	//	stop
	void StopAllSound();
	void StopBGM();
	void StopSE();
	void DeleteAllSound();

	//	Getter 
	//	範囲は0.0f～1.0f
	float GetBGMVolume() { return bgmVolume; }
	//	範囲は0.0f～1.0f
	float GetSEVolume() { return seVolume; }
	//	範囲は0.0f～1.0f
	float GetMasterVolume() { return masterVolume; }
};

