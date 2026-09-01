#include "ResultScene.h"
#include <DxLib.h>
#include "GameDefine.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
	DeleteGraph(m_p1WinHandle);
	DeleteGraph(m_p2WinHandle);
	DeleteGraph(m_winnerHandle);
}

void ResultScene::Init(GameData& data)
{
	m_p1WinHandle = LoadGraph("UI/P1.png");
	m_p2WinHandle = LoadGraph("UI/P2.png");
	m_winnerHandle = LoadGraph("UI/WINNER.png");
	m_buttonAHandle = LoadGraph("UI/controller/buttonA.png");

	m_winner.playerNum = 0;

	if (data.p1Point == 2)
	{
		m_winner.playerNum = 1;
		m_winner.chara = data.p1Chara;
	}
	else if (data.p2Point == 2)
	{
		m_winner.playerNum = 2;
		m_winner.chara = data.p2Chara;
	}

	// 勝者を確定してから、表示するキャラクターのアニメーションを初期化する
	m_winChara = std::make_unique<WinChara>();
	m_winChara->Init(m_winner.chara);
}

void ResultScene::Update(GameData& data, const Pad& _pad, const Pad& _pad2)
{
	m_winChara->Update();
	
	// Aボタンを押されたら仮でシーン戻る
	if (_pad.Trigger(ButtonType::A) || _pad2.Trigger(ButtonType::A))
	{
		ChangeScene(SceneID::Title);
	}
}

void ResultScene::Draw(GameData& data)
{
	// 背景色
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x15152b, true);

	m_winChara->Draw(300, Game::kScreenHeight / 2, false);
	
	DrawGraph(750, 150, m_winnerHandle, true);

	DrawGraph(1400, 750, m_buttonAHandle, true);

	// P1が勝利
	if (m_winner.playerNum == 1)
	{
		DrawGraph(900, 400, m_p1WinHandle, true);
	}
	else if (m_winner.playerNum == 2)
	{
		DrawGraph(900, 400, m_p2WinHandle, true);
	}
}
