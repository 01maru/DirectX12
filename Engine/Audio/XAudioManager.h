#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <map>

#pragma comment(lib,"xaudio2.lib")

#pragma region Struct

//	音データ
struct SoundData {
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	unsigned int bufferSize;

	float volume = 1.0f;
};

//	再生中の音データ
struct SoundVoicePtr {
	std::string dataKey;
	IXAudio2SourceVoice* ptr = nullptr;
};

#pragma endregion

class XAudioManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//	音量調節用Enum
	enum SoundType {
		Master,
		BGM,
		SE,
	};
private:
	//	Imgui用
	bool isDebug_ = false;

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;

	//	soundのデータ配列
	std::map<std::string, SoundData, std::less<>> data_;

	std::vector<SoundVoicePtr> soundEffectPtr;
	std::vector<SoundVoicePtr> bgmsoundPtr;
	std::vector<SoundVoicePtr> debugSoundPtr_;

	//	volume
	float bgmVolume = 1.0f;
	float seVolume = 1.0f;
	float masterVolume = 1.0f;

	float pitchRatio = 1.0f;

private:	//	関数
	void UnloadSoundData(SoundData* soundData);
	void ChangeVolume(float volume, SoundType type);
	void StopDebugSound();
	void StopSound(const std::string& soundName);

	float LoadVolume(const std::string& filename);
	void LoadAllValumeData();
	void SaveVolume();

	void PlayDebugSoundWave(const std::string& soundName, SoundType type, bool loop = false, bool isDebug = false);

	XAudioManager() {};
	~XAudioManager() {};
public:
	static XAudioManager* GetInstance();
	XAudioManager(const XAudioManager& obj) = delete;
	XAudioManager& operator=(const XAudioManager& obj) = delete;

	void Initialize();
	void Finalize();
	void Update();
	void ImguiUpdate();

	//	load
	void LoadSoundWave(const std::string& filename);
	//	play
	void PlaySoundWave(const std::string& soundName, SoundType type, bool loop = false);

	//	volume変更用(Optionとかで)
	void VolumeUpdate(SoundType type, int inputValue);

	//void ChangeAllPitchRatio(float pitch);

	//	stop
	void StopAllSound();
	void StopBGM();
	void StopSE();

	//	delete
	void DeleteAllSound();
	void DeleteSoundData(const std::string& soundName);

	//	Getter 
	//	範囲は0.0f～1.0f
	float GetBGMVolume() { return bgmVolume; }
	//	範囲は0.0f～1.0f
	float GetSEVolume() { return seVolume; }
	//	範囲は0.0f～1.0f
	float GetMasterVolume() { return masterVolume; }
};

