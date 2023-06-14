#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <map>
#include <memory>

#pragma comment(lib,"xaudio2.lib")

#pragma region Struct

//	���f�[�^
struct SoundData {
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	size_t bufferSize;

	float volume = 1.0f;
};

//	�Đ����̉��f�[�^
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
	//	���ʒ��ߗpEnum
	enum SoundType {
		Master,
		BGM,
		SE,
	};
private:
	//	Imgui�p
	bool isDebug_ = false;

	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	//	sound�̃f�[�^�z��
	std::map<std::string, SoundData, std::less<>> data_;

	std::vector<SoundVoicePtr> sePtr_;
	std::vector<SoundVoicePtr> bgmPtr_;
	std::vector<SoundVoicePtr> debugSoundPtr_;

	//	volume
	float masterVolume_ = 1.0f;
	float bgmVolume_ = 1.0f;
	float seVolume_ = 1.0f;

	float pitchRatio_ = 1.0f;

private:	//	�֐�
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

	//	volume�ύX�p(Option�Ƃ���)
	void VolumeUpdate(SoundType type, int32_t inputValue);

	//void ChangeAllPitchRatio(float pitch);

	//	stop
	void StopAllSound();
	void StopBGM();
	void StopSE();

	//	delete
	void DeleteAllSound();
	void DeleteSoundData(const std::string& soundName);

	//	Getter 
	//	�͈͂�0.0f�`1.0f
	float GetBGMVolume() { return bgmVolume_; }
	//	�͈͂�0.0f�`1.0f
	float GetSEVolume() { return seVolume_; }
	//	�͈͂�0.0f�`1.0f
	float GetMasterVolume() { return masterVolume_; }
};

