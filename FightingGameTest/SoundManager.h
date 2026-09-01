#pragma once
enum class SoundType
{
	Hit,
	Guard,
	Whiff,
	KO,
	Max
};

class SoundManager
{
private:
	int m_soundHandle[static_cast<int>(SoundType::Max)] = {};
public:
	SoundManager();
	~SoundManager();

	// コピーと代入を禁止
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	// インスタンスを取得する静的関数
	static SoundManager& Instance();

	void Init();
	void PlaySE(SoundType _soundType);
};