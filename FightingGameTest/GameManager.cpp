#include "GameManager.h"
#include "SoundManager.h"

namespace
{
	// 試合が始まるまでの時間
	// レディファイトみたいなのを出すための時間
	constexpr int kStartPhaseFrame = 140;
	// 次の試合までの時間
	constexpr int kNextPhaseFrame = 180;
}

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::Init(const std::unique_ptr<Player>& _p1, const std::unique_ptr<Player>& _p2)
{
	// プレイヤー取得
	m_p1 = _p1.get();
	m_p2 = _p2.get();

	// 変数の初期化
	m_winPlayer = 0;
	m_isFinish = false;
	m_nextPhaseFrameTimer = 0;
	m_nextPhase = false;
	m_p1Point = false;
	m_p2Point = 0;
	m_isStart = true;
	m_startFrameTimer = 0;
	m_isPlusPoint = false;
	m_is2Point = false;
	m_isPlayKOSE = false;
}

void GameManager::Update(GameData& _data)
{
	// スタートの終了
	if (m_isStart == true)
	{
		m_isPlayKOSE = false;
		m_startFrameTimer++;
		if (m_startFrameTimer > kStartPhaseFrame)
		{
			m_isStart = false;
		}
	}

	// 勝利したプレイヤーを決定する
	if (m_p1->GetPlayerHP() <= 0 && m_p2->GetPlayerHP() <= 0)
	{
		// doubleKO
		m_winPlayer = 3;
		if (!m_isPlusPoint)
		{
			m_isPlusPoint = true;
			_data.p1Point++;
			_data.p2Point++;
		}
	}
	else if (m_p2->GetPlayerHP() <= 0)
	{
		// 1PWIN
		if (!m_isPlusPoint)
		{
			m_isPlusPoint = true;
			_data.p1Point++;
		}
		m_winPlayer = 1;
	}
	else if (m_p1->GetPlayerHP() <= 0)
	{
		// 2PWiN
		if (!m_isPlusPoint)
		{
			m_isPlusPoint = true;
			_data.p2Point++;
		}
		m_winPlayer = 2;
	}
	CheckFinish();

	if (m_isFinish == true)
	{
		if (!m_isPlayKOSE)
		{
			m_isPlayKOSE = true;
			PlayKOSE();
		}
		m_nextPhaseFrameTimer++;
		if (m_nextPhaseFrameTimer > kNextPhaseFrame)
		{
			m_isFinish = false;
			m_nextPhase = true;
			m_nextPhaseFrameTimer = 0;
			m_winPlayer = 0;
			m_isStart = true;
			m_startFrameTimer = 0;
			m_isPlusPoint = false;
			_data.roundFrame = 0;
			// 2本先取したらゲームを終了
			if (_data.p1Point == 2 || _data.p2Point == 2)
			{
				m_is2Point = true;
			}
		}
	}

	if (!m_isStart)
	{
		_data.roundFrame++;
	}
}

void GameManager::Draw()
{

}

void GameManager::CheckFinish()
{
	if (m_winPlayer > 0)
	{
		m_isFinish = true;
		
	}
}

void GameManager::SetNextPhase(bool _nextPhase)
{
	m_nextPhase = _nextPhase;
}

void GameManager::PlayStartSE()
{
	SoundManager::Instance().PlaySE(SoundType::KO);
}

void GameManager::PlayKOSE()
{
	SoundManager::Instance().PlaySE(SoundType::KO);
}

