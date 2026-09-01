#include "SoundManager.h"
#include "DxLib.h"

SoundManager::SoundManager()
{
	for (int i = 0; i < static_cast<int>(SoundType::Max); i++)
	{
		m_soundHandle[i] = -1;
	}
}

SoundManager::~SoundManager()
{
	for (int i = 0; i < static_cast<int>(SoundType::Max); i++)
	{
		DeleteSoundMem(m_soundHandle[i]);
	}
}

SoundManager& SoundManager::Instance()
{
	// TODO: return ステートメントをここに挿入します
	static SoundManager instance;
	return instance;
}

void SoundManager::Init()
{
	m_soundHandle[static_cast<int>(SoundType::Hit)] = LoadSoundMem("Sound/SE/hit.mp3");
	m_soundHandle[static_cast<int>(SoundType::Guard)] = LoadSoundMem("Sound/SE/guard.mp3");
	m_soundHandle[static_cast<int>(SoundType::Whiff)] = LoadSoundMem("Sound/SE/whiff.mp3");
	m_soundHandle[static_cast<int>(SoundType::KO)] = LoadSoundMem("Sound/SE/KO.mp3");
}

void SoundManager::PlaySE(SoundType _soundType)
{
	int idx = static_cast<int>(_soundType);

	// エラーの場合はリターン
	if (idx < 0 || idx >= static_cast<int>(SoundType::Max)) return;
	if (m_soundHandle[idx] == -1) return;

	PlaySoundMem(m_soundHandle[idx], DX_PLAYTYPE_BACK, true);
}
