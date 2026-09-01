#include "UIManager.h"
#include "DxLib.h"
namespace
{

	// HPバー
	constexpr Box kHpBar = { 20, 100, 680, 30 };
	constexpr BoxData kP1BarBack =
	{
		static_cast<int>(kHpBar.x),
		static_cast<int>(kHpBar.y),
		static_cast<int>(kHpBar.width + kHpBar.x),
		static_cast<int>(kHpBar.height + kHpBar.y)
	};
	constexpr BoxData kP2BarBack =
	{
		static_cast<int>(Game::kScreenWidth - (kHpBar.x + kHpBar.width)),
		static_cast<int>(kHpBar.y),
		static_cast<int>(Game::kScreenWidth - (kHpBar.x)),
		static_cast<int>(kHpBar.height + kHpBar.y)
	};

	// HPバーの色
	constexpr unsigned int kOutLineCr = 0x0000ff;
	constexpr unsigned int kHpBarBackCr = 0x000000;
	constexpr unsigned int kHpBarCr = 0x00ff00;

	// KO
	// 白く点滅させる間隔
	constexpr int kWhiteSpan = 5;
	// 白を表示している時間
	constexpr int kWhiteFrame = 4;
	constexpr int kWhiteOutTime = 4;
	constexpr int  kDrawKOFrame = 60;

	// Startのとき
	constexpr int kReadyFrame = 90;
	constexpr int kStartFrame = 30;

	// Pointを表示する座標
	constexpr int kP1PointPosX = 550;
	constexpr int kP1PointPosY = 100;
	constexpr int kP2PointPosX = Game::kScreenWidth - kP1PointPosX;
	constexpr int kP2PointPosY = kP1PointPosY;
}


UIManager::UIManager()
{
}

UIManager::~UIManager()
{
	DeleteGraph(m_readyHandle);
	DeleteGraph(m_fightHandle);
	DeleteGraph(m_p1WinHandle);
	DeleteGraph(m_p2WinHandle);
	DeleteGraph(m_KOHandle);
}

void UIManager::Init(const std::unique_ptr<Player>& _p1, const std::unique_ptr<Player>& _p2, const std::unique_ptr<GameManager>& _gm)
{
	// プレイヤー
	m_p1 = _p1.get();
	m_p2 = _p2.get();
	m_p1MaxHp = m_p1->GetPlayerMaxHP();
	m_p2MaxHp = m_p2->GetPlayerMaxHP();
	// ゲームマネージャー
	m_gm = _gm.get();
	// タイムマネージャー
	m_Time = std::make_unique<TimeUIManager>();
	m_Time->Init();
	// HPUIマネージャー
	m_HpBar = std::make_unique<HPUIManager>();
	m_HpBar->Init(m_p1MaxHp, m_p2MaxHp);
	// ポイントUI
	m_P1Point = std::make_unique<PointUI>();
	m_P2Point = std::make_unique<PointUI>();
	m_P1Point->Init();
	m_P2Point->Init();

	
	m_WhiteOutFrameTimer = 0;
	m_KOFrameTimer = 0;
	m_whiteOutIdx = 0;
	m_startFrameTimer = 0;

	// 画像の読み込み
	m_readyHandle = LoadGraph("UI/Ready1.png");
	m_fightHandle = LoadGraph("UI/Fight0.png");
	m_p1WinHandle = LoadGraph("UI/P1Wins.png");
	m_p2WinHandle = LoadGraph("UI/P2Wins.png");
	m_KOHandle = LoadGraph("UI/KO0.png");
}

void UIManager::SecondInit()
{
	m_WhiteOutFrameTimer = 0;
	m_KOFrameTimer = 0;
	m_whiteOutIdx = 0;
	m_startFrameTimer = 0;
}

void UIManager::Update(GameData& _data)
{
	m_data = _data;
	UpdateHp();
	m_Time->Update(_data.roundFrame);
	m_P1Point->Update(_data.p1Point);
	m_P2Point->Update(_data.p2Point);
}

void UIManager::UpdateHp()
{
	// プレイヤーのHPと最大HPからHPのパーセントを求める
	int p1Hp = m_p1->GetPlayerHP();

	int p2Hp = m_p2->GetPlayerHP();

	m_HpBar->Update(p1Hp, p2Hp);
}

void UIManager::Draw()
{
	FinishPhase();
	//DrawHpBar();
	// 時間の表示
	m_Time->Draw();
	m_HpBar->Draw();
	//DrawFormatString(800, 200, 0xff00ff, "P1:%d, P2:%d", m_data.p1Point, m_data.p2Point);

	m_P1Point->Draw(kP1PointPosX, kP1PointPosY);
	m_P2Point->Draw(kP2PointPosX, kP2PointPosY);
}

void UIManager::DrawFront()
{
	// ゲームが終わった時の処理
	FinishPhaseFront();
	// ゲームが始まるときの処理
	StartGame();
}



void UIManager::DrawHpBar()
{
	// プレイヤーのHPを取得し、最大HPとの割合を求める
	

	// HPバーの位置を設定していく
	// BoxData { x1, y1, x2, y2 }
	BoxData p1Bar =
	{
		static_cast<int>(kHpBar.x + kHpBar.width * (1.0f - m_p1HpPer)),
		static_cast<int>(kHpBar.y),
		static_cast<int>(kHpBar.width + kHpBar.x),
		static_cast<int>(kHpBar.height + kHpBar.y)
	};
	BoxData p2Bar =
	{
		static_cast<int>(Game::kScreenWidth - (kHpBar.x + kHpBar.width)),
		static_cast<int>(kHpBar.y),
		static_cast<int>(Game::kScreenWidth - kHpBar.x - kHpBar.width * (1.0f - m_p2HpPer)),
		static_cast<int>(kHpBar.height + kHpBar.y)
	};

	/*// HPバーの後ろ側
	DrawBox(kP1BarBack.x1, kP1BarBack.y1, kP1BarBack.x2, kP1BarBack.y2, kHpBarBackCr, true, false);
	DrawBox(kP2BarBack.x1, kP2BarBack.y1, kP2BarBack.x2, kP2BarBack.y2, kHpBarBackCr, true, false);

	// HPバー本体
	DrawBox(p1Bar.x1, p1Bar.y1, p1Bar.x2, p1Bar.y2, kHpBarCr, true, false);
	DrawBox(p2Bar.x1, p2Bar.y1, p2Bar.x2, p2Bar.y2, kHpBarCr, true, false);*/

	// HPバーの後ろ側
	DrawRectGraph
	(
		kP1BarBack.x1, kP1BarBack.y1,
		0, 0,
		kP1BarBack.x2, kP1BarBack.y2,
		m_hpBarOutLineHandle, 
		true, 
		false
	);
	
	DrawRectGraph
	(
		kP2BarBack.x1, kP2BarBack.y1,
		0, 0,
		kP2BarBack.x2, kP2BarBack.y2,
		m_hpBarOutLineHandle,
		true,
		true
	);

	// HPバー本体
	//DrawGraph(0, 0, m_hpBarHandle, true);
	

}

void UIManager::FinishPhase()
{
	if (m_gm->GetIsFinish())
	{
		// 仮
		// 画面を白く点滅させてKOの文字→Player1 WINみたいな感じ
		m_WhiteOutFrameTimer++;
		if (m_WhiteOutFrameTimer >= kWhiteSpan && m_WhiteOutFrameTimer <= kWhiteSpan + kWhiteFrame && m_whiteOutIdx <= kWhiteOutTime)
		{
			DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xcc0000, true);
		}
		else if (m_WhiteOutFrameTimer > kWhiteSpan + kWhiteFrame && m_whiteOutIdx <= kWhiteOutTime)
		{
			m_WhiteOutFrameTimer = 0;
			m_whiteOutIdx++;
		}
	}
}

void UIManager::FinishPhaseFront()
{
	if (m_gm->GetIsFinish())
	{
		m_KOFrameTimer++;
		int winPlayer = m_gm->GetWinPlayer();
		// 文字の表示(仮)
		// KOの表示
		if (m_KOFrameTimer <= kDrawKOFrame)
		{
			DrawGraph(0, 0, m_KOHandle, true);
		}
		// 勝利プレイヤーの表示
		else if (m_KOFrameTimer > kDrawKOFrame)
		{
			if (winPlayer == 1)
			{
				DrawGraph(0, 0, m_p1WinHandle, true);
			}
			else if (winPlayer == 2)
			{
				DrawGraph(0, 0, m_p2WinHandle, true);
			}
			else if (winPlayer == 3)
			{
				DrawFormatString(800, 500, 0xffff00, "DUECE");
			}
		}
	}
}

void UIManager::StartGame()
{
	// READY? → FIGHT!!
	if (m_gm->GetIsStart() == true)
	{
		m_startFrameTimer++;
		if (m_startFrameTimer < kReadyFrame)
		{
			DrawGraph(0, 0, m_readyHandle, true);
		}
		else if (m_startFrameTimer >= kReadyFrame)
		{
			DrawGraph(0, 0, m_fightHandle, true);
		}
	}
}



