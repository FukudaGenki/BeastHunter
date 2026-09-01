#include "DxLib.h"
#include "TitleScene.h"
#include "GameDefine.h"
namespace
{
	constexpr float kTitlePosX = 800;
	constexpr float kTitlePosY = 250;
	constexpr int kInitBlendIdx = 128;

	constexpr int kBlinkSpanFrame = 25;
}

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	DeleteGraph(m_titleHandle);
	DeleteGraph(m_pressAHandle);
}

void TitleScene::Init(GameData& data)
{
	m_titleHandle = LoadGraph("UI/title.png");
	GetGraphSize(m_titleHandle, &m_titleSizeX, &m_titleSizeY);

	m_pressAHandle = LoadGraph("UI/PressA.png");

	// 変数の初期化
	m_blendIdx = kInitBlendIdx;
	m_frameTimer = 0;
}

void TitleScene::Update(GameData& data, const Pad& _pad, const Pad& _pad2)
{
	m_frameTimer++;

	// 点滅するタイミング
	bool isBlink = m_frameTimer % kBlinkSpanFrame == 0;
	if (isBlink)
	{
		if (m_isPressAActive)
		{
			m_isPressAActive = false;
		}
		else
		{
			m_isPressAActive = true;
		}
	}

	bool isBlendIdxMax = m_blendIdx >= 255;
	if (!isBlendIdxMax) m_blendIdx+=2;
	// Aボタンを押されたら仮でシーン戻る
	if (_pad.Trigger(ButtonType::A) || _pad2.Trigger(ButtonType::A))
	{
		ChangeScene(SceneID::CharaSelect);
	}
}

void TitleScene::Draw(GameData& data)
{
	// 背景色
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x15152b, true);

	// フェード
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blendIdx);
	// タイトルの描画
	DrawCenter(m_titleHandle, kTitlePosX, kTitlePosY, m_titleSizeX, m_titleSizeY);
	// ボタンの描画
	if(m_isPressAActive)DrawGraph(0, 0, m_pressAHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void TitleScene::DrawCenter(int _handle, float _posX, float _posY, int _sizeX, int _sizeY)
{
	if (_sizeX <= 0 || _sizeY <= 0) return;
	int x = static_cast<int>(_posX);
	int y = static_cast<int>(_posY);
	int drawX = x - x / 2;
	int drawY = y - y / 2;

	DrawGraph(drawX, drawY, _handle, true);
}
