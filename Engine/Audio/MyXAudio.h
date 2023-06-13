#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <map>

#pragma comment(lib,"xaudio2.lib")

//	���f�[�^
struct SoundData {
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	unsigned int bufferSize;

	float volume = 1.0f;
};

//	�Đ����̉��f�[�^
struct SoundVoicePtr {
	std::string soundname;
	IXAudio2SourceVoice* ptr = nullptr;
	//float volume = 1.0f;
};

class MyXAudio
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

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;

	//	sound�̃f�[�^�z��
	std::map<std::string, SoundData, std::less<>> data_;

	std::vector<SoundVoicePtr> soundEffectPtr;
	std::vector<SoundVoicePtr> bgmsoundPtr;

	//	volume
	float bgmVolume = 1.0f;
	float seVolume = 1.0f;
	float masterVolume = 1.0f;

	float pitchRatio = 1.0f;

private:	//	�֐�
	void SoundUnload(SoundData* soundData_);

	MyXAudio() {};
	~MyXAudio() {};
public:
	static MyXAudio* GetInstance();
	MyXAudio(const MyXAudio& obj) = delete;
	MyXAudio& operator=(const MyXAudio& obj) = delete;

	void Initialize();
	void Finalize();
	void Update();
	void ImguiUpdate();

	//	load
	void LoadSoundWave(const std::string& filename);
	//	play
	void PlaySoundWave(const std::string& soundName, SoundType type, bool loop = false);

	//	volume�ύX�p(Option�Ƃ���)
	void VolumeUpdate(SoundType type);
	void ChangeVolume(float volume, SoundType type);
	//void ChangeAllPitchRatio(float pitch);

	//	stop
	void StopAllSound();
	void StopBGM();
	void StopSE();

	//	delete
	void DeleteAllSound();

	//	Getter 
	//	�͈͂�0.0f�`1.0f
	float GetBGMVolume() { return bgmVolume; }
	//	�͈͂�0.0f�`1.0f
	float GetSEVolume() { return seVolume; }
	//	�͈͂�0.0f�`1.0f
	float GetMasterVolume() { return masterVolume; }
};

