#include "SceneMain.h"
#include "Vec2.h"
#include "GameDefine.h"
class Nova;
class Shartel;
class Player;
class HadouBullet;

namespace
{
	constexpr int kPlayerNum = 2;
	constexpr int k1PIdx = 0;
	constexpr int k2PIdx = 1;
	constexpr int k1PInitPos = 200;
	constexpr int kInitPlayerDistance = 720;

	constexpr float kHadouPosY = -100.0f;

	// カメラスクロール
	constexpr float kDistance = 1000.0f;
}

SceneMain::SceneMain()
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init(GameData& data)
{
	for (int i = 0; i < kPlayerNum; i++)
	{
		// インスタンスの取得
		if (i == 0)
		{
			if (data.p1Chara == Chara::Shartel) m_Player[i] = std::make_unique<Shartel>();
			else m_Player[i] = std::make_unique <Nova>();
		}
		if (i == 1)
		{
			if (data.p2Chara == Chara::Shartel) m_Player[i] = std::make_unique<Shartel>();
			else m_Player[i] = std::make_unique <Nova>();
		}
		// 初期化処理
		m_Player[i]->Init(static_cast<float>(k1PInitPos)+ (kInitPlayerDistance) * i, i + 1);
	}
	m_Bg = std::make_unique<Bg>();
	m_Bg->Init();
	m_GameManager = std::make_unique<GameManager>();
	m_GameManager->Init(m_Player[k1PIdx], m_Player[k2PIdx]);
	m_UI = std::make_unique<UIManager>();
	m_UI->Init(m_Player[k1PIdx], m_Player[k2PIdx], m_GameManager);
	m_ColCheck = std::make_unique<ColCheck>();
	m_ColCheck->Init(m_Player[k1PIdx], m_Player[k2PIdx]);
	m_Effect = std::make_unique<EffectManager>();
	m_Effect->Init();

	// 対戦ごとの値だけを初期化し、セレクトシーンで選んだキャラクターは保持する
	data.p1Point = 0;
	data.p2Point = 0;
	data.roundFrame = 0;
}

void SceneMain::Update(GameData& data, const Pad& _pad, const Pad& _pad2)
{
	// プレイヤーの更新
	PlayerUpdate(_pad, _pad2);
	// 背景の処理
	m_Bg->Update();
	// 押し戻しや攻撃の当たり判定
	m_ColCheck->Update();
	CheckEffectGene();
	// エフェクトの更新
	m_Effect->Update();
	// 波動拳など
	GeneBullet();
	// プレイヤーの座標を更新
	m_Player[k1PIdx]->UpdateMove(m_GameManager->GetIsStart());
	m_Player[k2PIdx]->UpdateMove(m_GameManager->GetIsStart());
	// UIの更新
	m_UI->Update(data);
	m_GameManager->Update(data);

	GoNextPhase();
}

void SceneMain::Draw(GameData& data)
{
	m_Bg->Draw();
	m_UI->Draw();
	for (int i = 0; i < kPlayerNum; i++)
	{
		if (m_Hadou[i])
		{
			m_Hadou[i]->Draw();
		}
		m_Player[i]->Draw();
	}
	//m_Bg->DrawFront();
	m_Effect->Draw();
	m_UI->DrawFront();

#ifdef _DEBUG
	
#endif
}

float SceneMain::GetScrollX(int _dir)
{
	/*float scrollX = 0;
	float p1Pos = m_Player[k1PIdx]->GetCenterPos().x;
	float p2Pos = m_Player[k2PIdx]->GetCenterPos().x;
	float centerPos = (p1Pos + p2Pos) * 0.5f;*/

	float scrollX = 0;

	float p1Move = m_Player[k1PIdx]->GetMoveX();
	float p2Move = m_Player[k2PIdx]->GetMoveX();
	Stage stagePos = m_Bg->GetStagePos();
	// プレイヤーが画面端にいるかどうかを取得する変数
	bool isRightEdge = m_Player[k1PIdx]->GetIsRightEdge() || m_Player[k2PIdx]->GetIsRightEdge();
	// プレイヤーが画面端にいるかどうかを取得する変数
	bool isLeftEdge = m_Player[k1PIdx]->GetIsLeftEdge() || m_Player[k2PIdx]->GetIsLeftEdge();
	// プレイヤーのX中心座標
	float p1Pos = m_Player[k1PIdx]->GetCenterPos().x;
	float p2Pos = m_Player[k2PIdx]->GetCenterPos().x;
	// プレイヤーが一定距離にいるかどうか
	bool isDistance = abs(p2Pos - p1Pos) <= kDistance;
	// プレイヤーが同じ方向に歩いているかどうか
	bool isMoveSameDir = p1Move * p2Move < 0;

	if(_dir == 1) scrollX = (p2Move - p1Move) * 0.5f;
	else if(_dir == -1) scrollX = (p1Move - p2Move) * 0.5f;
	
	
	if ((stagePos.left <= static_cast<float>(Game::kStageLeft) && scrollX >= 0.0f)|| // カメラがステージの左端にいったとき
		(stagePos.right >= static_cast<float>(Game::kStageRight) && scrollX <= 0.0f)|| // カメラがステージの右端に行ったとき
		(isLeftEdge && scrollX <= 0.0f) || // プレイヤーが画面端に触れているとき、引っ張って無理やり移動をできないようにする
		(isRightEdge && scrollX >= 0.0f) || // プレイヤー画面恥なら
		(isDistance && !isMoveSameDir)) // 一定の距離離れているかどうか
	{
		return 0;
	}
	else
	{
		return scrollX;
	}
	return scrollX;
}

void SceneMain::GoNextPhase()
{
	if (!m_GameManager->GetGoNextPhase()) return;
	m_GameManager->SetNextPhase(false);
	// どちらかが2勝していた場合、リザルトシーンへ
	if (m_GameManager->GetIs2Point())
	{
		ChangeScene(SceneID::Result);
	}
	else
	{
		// 次の試合へ行く処理
		// 必要なものの初期化
		for (int i = 0; i < kPlayerNum; i++)
		{
			// 初期化処理
			//m_Player[i]->Init(static_cast<float>(k1PInitPos) + (kInitPlayerDistance)*i);
			m_Player[i]->SecondInit(static_cast<float>(k1PInitPos) + (kInitPlayerDistance)*i);
		}
		m_Bg->SecondInit();
		//m_GameManager->Init(m_Player[k1PIdx], m_Player[k2PIdx]);
		m_UI->SecondInit();
		m_ColCheck->Init(m_Player[k1PIdx], m_Player[k2PIdx]);
		m_Effect->Init();
	}
}

void SceneMain::PlayerUpdate(const Pad& _pad, const Pad& _pad2)
{
	m_Player[k1PIdx]->Update(_pad, m_GameManager->GetIsFinish());
	m_Player[k2PIdx]->Update(_pad2, m_GameManager->GetIsFinish());

	int p1Dir = 0;
	int p2Dir = 0;

	// プレイヤーどうしの位置関係（向き）を決定する
	if (m_Player[k1PIdx]->GetPos().x < m_Player[k2PIdx]->GetPos().x)
	{
		// プレイヤー1が左側・右向きのとき
		p1Dir = 1;
		p2Dir = -1;
	}
	else if (m_Player[k1PIdx]->GetPos().x > m_Player[k2PIdx]->GetPos().x)
	{
		// プレイヤー1が右側・左向きの時
		p1Dir = -1;
		p2Dir = 1;
	}
	m_Player[k1PIdx]->SetRightLeft(p1Dir);
	m_Player[k2PIdx]->SetRightLeft(p2Dir);

	m_Player[k1PIdx]->SetScrollX(GetScrollX(p1Dir));
	m_Player[k2PIdx]->SetScrollX(GetScrollX(p1Dir));
	m_Bg->SetScrollX(GetScrollX(p1Dir));
}

void SceneMain::GeneBullet()
{
	for (int i = 0; i < kPlayerNum; i++)
	{
		if (m_Player[i]->GetIsHadou())
		{
			if (!m_Hadou[i])
			{
				m_Hadou[i] = std::make_unique<HadouBullet>();
				m_Hadou[i]->Init(m_Player[i]->GetCenterPos().x, 
				m_Player[i]->GetCenterPos().y + kHadouPosY, m_Player[i]->GetRightLeft());
			}
		}

		if (m_Hadou[i])
		{
			m_Hadou[i]->Update();

			// 弾とプレイヤーの当たり判定
			// 当てる敵への当たり判定
			int enePIdx = -1;

			if (i == k1PIdx) enePIdx = k2PIdx;
			else if (i == k2PIdx) enePIdx = k1PIdx;

			// 上段ガードの場合
			if (IsColBullet(i, enePIdx, ColType::GuardTop))
			{
				m_Player[enePIdx]->IsGuard(m_Hadou[i]->GetDamage());
				m_Hadou[i].reset();
			}
			// やられの場合
			else if (IsColBullet(i, enePIdx, ColType::Hurt))
			{
				m_Player[enePIdx]->IsDamaged(m_Hadou[i]->GetDamage(), AttackType::S);
				m_Hadou[i].reset();
			}
			// 画面端までいったとき
			else if (m_Hadou[i]->GetPos().x > Game::kScreenWidth || m_Hadou[i]->GetPos().x < 0) // ここ０は直す（仮）
			{
				// 画面端まで行けばデストロイ
				m_Hadou[i].reset();
			}
		}
		// 球数増えればここに追加でOk
	}
}

void SceneMain::CheckEffectGene()
{
	IsColData p1Hit = m_ColCheck->GetP1Hit();
	IsColData p1Guard = m_ColCheck->GetP1Guard();
	IsColData p2Hit = m_ColCheck->GetP2Hit();
	IsColData p2Guard = m_ColCheck->GetP2Guard();

	// P1のエフェクト生成
	// 攻撃されたときに攻撃エフェクト
	if (p1Hit.isCol)
	{
		m_Effect->SetPlay(1, EffectType::Hit, p1Hit.colPos);
	}
	// ガードしたときのガードエフェクト
	if (p1Guard.isCol)
	{
		m_Effect->SetPlay(1, EffectType::Guard, p1Guard.colPos);
	}

	// P2のエフェクト生成
	// 攻撃されたときに生成する攻撃エフェクト
	if (p2Hit.isCol)
	{
		m_Effect->SetPlay(2, EffectType::Hit, p2Hit.colPos);
	}
	// ガードしたときのガードエフェクト
	if (p2Guard.isCol)
	{
		m_Effect->SetPlay(2, EffectType::Guard, p2Guard.colPos);
	}

}

bool SceneMain::IsColBullet(int _pNum, int _enePNum, ColType _colType)
{
	std::vector<Rect> p = m_Player[_enePNum]->GetCol(_colType);
	for (int i = 0; i < p.size(); i++)
	{
		if (m_Hadou[_pNum]->GetCol().IsCollision(p[i]))
		{
			return true;
		}
	}
	return false;
}
