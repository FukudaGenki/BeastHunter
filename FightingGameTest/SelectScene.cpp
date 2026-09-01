#include "DxLib.h"
#include <string>
#include "SelectScene.h"
#include "GameDefine.h"

namespace
{
	float p1PosX = 400;
	float p1PosY = 450;
	float p2PosX = Game::kScreenWidth - p1PosX;
	float p2PosY = p1PosY;

	int kStartDelayFrame = 30;
}

SelectScene::SelectScene()
{
}

SelectScene::~SelectScene()
{
	delete m_pFireEffectAnim;
}

void SelectScene::Init(GameData& data)
{
	m_p1Chara = std::make_unique<SelectChara>();
	m_p1Chara->Init();

	m_p2Chara = std::make_unique<SelectChara>();
	m_p2Chara->Init();

	m_Controller = std::make_unique<ControllerUI>();
	m_Controller->Init();

	m_Controller2 = std::make_unique<ControllerUI>();
	m_Controller2->Init();

	// 火のエフェクトのアニメーションをロードする
	m_fireEffectHandle = LoadGraph("effect/fireEffect.png");

	m_pFireEffectAnim = new Animation;
	m_pFireEffectAnim->Init(m_fireEffectHandle, 10, 4, true);

	// 変数の初期化
	m_startFrameTimer = 0;
}

void SelectScene::Update(GameData& data, const Pad& _pad, const Pad& _pad2)
{
	// 選んでるプレイヤーを変える
	bool isP1ChangeCursor = (_pad.Trigger(ButtonType::Left) || _pad.Trigger(ButtonType::Right)) && !m_p1Select.isDecide;
	bool isP2ChangeCursor = (_pad2.Trigger(ButtonType::Left) || _pad2.Trigger(ButtonType::Right)) && !m_p2Select.isDecide;

	// 選んでいるキャラクターを決定
	bool isP1TriggerDecideButton = _pad.Trigger(ButtonType::A) && !m_p1Select.isDecide;
	bool isP2TriggerDecideButton = _pad2.Trigger(ButtonType::A) && !m_p2Select.isDecide;

	
	// キャラクターのセレクト
	if (isP1ChangeCursor)
	{
		if (m_p1Select.chara == Chara::Nova)
		{
			m_p1Select.chara = Chara::Shartel;
		}
		else if(m_p1Select.chara == Chara::Shartel)
		{
			m_p1Select.chara = Chara::Nova;
		}

		
	}

	if (isP2ChangeCursor)
	{
		if (m_p2Select.chara == Chara::Nova)
		{
			m_p2Select.chara = Chara::Shartel;
		}
		else if (m_p2Select.chara == Chara::Shartel)
		{
			m_p2Select.chara = Chara::Nova;
		}

		
	}

	// キャラクターの決定
	if (isP1TriggerDecideButton)
	{
		m_p1Select.isDecide = true;
		data.p1Chara = m_p1Select.chara;

		m_Controller->SetIsDecide(true);
	}

	if (isP2TriggerDecideButton)
	{
		m_p2Select.isDecide = true;
		data.p2Chara = m_p2Select.chara;

		m_Controller2->SetIsDecide(true);
	}

	// セレクトキャラのアップデート
	m_p1Chara->Update(m_p1Select.chara);
	m_p2Chara->Update(m_p2Select.chara);

	// 両方のキャラ選択が終わってるかどうか
	bool isBothPlayerDecideChara = m_p1Select.isDecide && m_p2Select.isDecide;

	if (isBothPlayerDecideChara)
	{
		m_startFrameTimer++;
	}

	if (m_startFrameTimer >= kStartDelayFrame)
	{
		ChangeScene(SceneID::Game);
	}
	else
	{
		// 火のエフェクトの更新処理
		m_pFireEffectAnim->Update();
	}

	m_Controller->Update();
	m_Controller2->Update();
}

void SelectScene::Draw(GameData& data)
{
	// 背景色
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x15152b, true);

	// 火のエフェクトを描画

	m_pFireEffectAnim->DrawCenter(Game::kScreenWidth / 2, Game::kScreenHeight / 2, false);

	m_Controller->Draw(p1PosX, p1PosY + 400);
	m_Controller2->Draw(p2PosX, p2PosY + 400);

	// 選んだキャラクターの描画
	m_p1Chara->Draw(p1PosX, p1PosY, false);
	m_p2Chara->Draw(p2PosX, p2PosY, true);

#ifdef _DEBUG
	// 選んだキャラクターの描画
	std::string p1Chara = "Nova";
	std::string p2Chara = "Nova";
	if (m_p1Select.chara == Chara::Shartel) p1Chara = "Shartel";
	if (m_p2Select.chara == Chara::Shartel) p2Chara = "Shartel";

	unsigned int p1Cr = 0xffffff;
	unsigned int p2Cr = 0xffffff;

	if (m_p1Select.isDecide) p1Cr = 0xff0000;
	if (m_p2Select.isDecide) p2Cr = 0xff0000;

	DrawFormatString(800, 200, 0xffff00, "SelectScene");
	DrawFormatString(200, 500, p1Cr, "P1:Chara :%7s", p1Chara.c_str());
	DrawFormatString(1000, 500, p2Cr, "P2:Chara :%7s", p2Chara.c_str());
#endif
}
