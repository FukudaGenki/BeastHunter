#include "ColCheck.h"

ColCheck::ColCheck()
{
}

ColCheck::~ColCheck()
{
}

void ColCheck::Init(const std::unique_ptr<Player>& _p1, const std::unique_ptr<Player>& _p2)
{
	m_p1 = _p1.get();
	m_p2 = _p2.get();
	// 初期化
	m_p1Hit.colPos = {};
	m_p1Guard.colPos = {};
	m_p2Hit.colPos = {};
	m_p2Guard.colPos = {};
}

void ColCheck::Update()
{
	// リセット
	m_p1Hit.isCol = false;
	m_p1Guard.isCol = false;
	m_p2Hit.isCol = false;
	m_p2Guard.isCol = false;

	// プレイヤーが画面端にいるかどうかを取得（ノックバックの処理で使用）
	bool p1Edge = m_p1->GetIsLeftEdge() || m_p1->GetIsRightEdge();
	bool p2Edge = m_p2->GetIsLeftEdge() || m_p2->GetIsRightEdge();

	// 攻撃のチェック
	// プレイヤー1が攻撃
	if (!m_p1->GetIsHitOnce())
	{
		// プレイヤーどうしの攻撃当たり判定を行う
		// プレイヤー1がプレイヤー2にダメージを与える場合

		// H: Hit、G: Guardの略

		IsColData topHTopG = IsCol1PTo2P(ColType::HitTop, ColType::GuardTop);
		IsColData middleHTopG = IsCol1PTo2P(ColType::HitMiddle, ColType::GuardTop);

		IsColData topHBottomG = IsCol1PTo2P(ColType::HitTop, ColType::GuardBottom);
		IsColData bottomHBottomG = IsCol1PTo2P(ColType::HitBottom, ColType::GuardBottom);

		IsColData topHHurt = IsCol1PTo2P(ColType::HitTop, ColType::Hurt);
		IsColData middleHHurt = IsCol1PTo2P(ColType::HitMiddle, ColType::Hurt);
		IsColData middleHBottomG = IsCol1PTo2P(ColType::HitMiddle, ColType::GuardBottom);
		IsColData bottomHHurt = IsCol1PTo2P(ColType::HitBottom, ColType::Hurt);
		IsColData bottomHGuardTop = IsCol1PTo2P(ColType::HitBottom, ColType::GuardTop);

		// 上段ガードが成功
		if (topHTopG.isCol || middleHTopG.isCol)
		{
			m_p2->IsGuard(m_p1->GetMainDamage());
			m_p1->SetIsHit(true);
			m_p2Guard.isCol = true;
			if (p2Edge) m_p1->SetIsHitBack(true);
			if (topHTopG.isCol) m_p2Guard = topHTopG;
			else m_p2Guard = middleHTopG;
		}
		// 下段ガードが成功
		else if (topHBottomG.isCol || bottomHBottomG.isCol)
		{
			m_p2->IsGuard(m_p1->GetMainDamage());
			m_p1->SetIsHit(true);
			m_p2Guard.isCol = true;
			if (p2Edge) m_p1->SetIsHitBack(true);
			if (topHBottomG.isCol) m_p2Guard = topHBottomG;
			else m_p2Guard = bottomHBottomG;
		}
		// やられ
		else if (topHHurt.isCol || middleHHurt.isCol || middleHBottomG.isCol || bottomHHurt.isCol || bottomHGuardTop.isCol)
		{
			m_p2->IsDamaged(m_p1->GetMainDamage(), m_p1->GetAttackType());
			m_p1->SetIsHit(true);
			m_p2Hit.isCol = true;
			if (p2Edge) m_p1->SetIsHitBack(true);
			if (topHHurt.isCol) m_p2Hit = topHHurt;
			else if (middleHHurt.isCol) m_p2Hit = middleHHurt;
			else if (middleHBottomG.isCol) m_p2Hit = middleHBottomG;
			else if (bottomHHurt.isCol) m_p2Hit = bottomHHurt;
			else m_p2Hit = bottomHGuardTop;
		}
	}

	// プレイヤー２が攻撃
	if (!m_p2->GetIsHitOnce())
	{
		// H: Hit、G: Guardの略

		IsColData topHTopG = IsCol2PTo1P(ColType::GuardTop, ColType::HitTop);
		IsColData middleHTopG = IsCol2PTo1P(ColType::GuardTop, ColType::HitMiddle);

		IsColData topHBottomG = IsCol2PTo1P(ColType::GuardBottom, ColType::HitTop);
		IsColData bottomHBottomG = IsCol2PTo1P(ColType::GuardBottom, ColType::HitBottom);

		IsColData topHHurt = IsCol2PTo1P(ColType::Hurt, ColType::HitTop);
		IsColData middleHHurt = IsCol2PTo1P(ColType::Hurt, ColType::HitMiddle);
		IsColData middleHBottomG = IsCol2PTo1P(ColType::GuardBottom, ColType::HitMiddle);
		IsColData bottomHHurt = IsCol2PTo1P(ColType::Hurt, ColType::HitBottom);
		IsColData bottomHGuardTop = IsCol2PTo1P(ColType::GuardTop, ColType::HitBottom);

		// プレイヤー2がプレイヤー1にダメージを与える場合
		// 上段ガードが成功
		if (topHTopG.isCol || middleHTopG.isCol)
		{
			m_p1->IsGuard(m_p2->GetMainDamage());
			m_p2->SetIsHit(true);
			m_p1Guard.isCol = true;
			if (p1Edge) m_p2->SetIsHitBack(true);
			if (topHTopG.isCol) m_p1Guard = topHTopG;
			else m_p1Guard = middleHTopG;
		}
		// 下段ガードが成功
		else if (topHBottomG.isCol || bottomHBottomG.isCol)
		{
			m_p1->IsGuard(m_p2->GetMainDamage());
			m_p2->SetIsHit(true);
			m_p1Guard.isCol = true;
			if (p1Edge) m_p2->SetIsHitBack(true);
			if (topHBottomG.isCol) m_p1Guard = topHBottomG;
			else m_p1Guard = bottomHBottomG;
		}
		// やられ
		else if (topHHurt.isCol || middleHHurt.isCol || middleHBottomG.isCol || bottomHHurt.isCol || bottomHGuardTop.isCol)
		{
			m_p1->IsDamaged(m_p2->GetMainDamage(), m_p2->GetAttackType());
			m_p2->SetIsHit(true);
			m_p1Hit.isCol = true;
			if (p1Edge) m_p2->SetIsHitBack(true);
			if (topHHurt.isCol) m_p1Hit = topHHurt;
			else if (middleHHurt.isCol) m_p1Hit = middleHHurt;
			else if (middleHBottomG.isCol) m_p1Hit = middleHBottomG;
			else if (bottomHHurt.isCol) m_p1Hit = bottomHHurt;
			else m_p1Hit = bottomHGuardTop;
		}
	}

	// 押し戻し----------------------------------------------------------
	// プレイヤーの座標
	Vec2 p1Pos = m_p1->GetPos();
	Vec2 p2Pos = m_p2->GetPos();
	// プレイヤーのX軸動き
	float p1Move = m_p1->GetVirtualMoveX();
	float p2Move = m_p2->GetVirtualMoveX();
	// プレイヤーの押し戻し判定の右側、左側の幅
	// プレイヤー1
	float p1Right = m_p1->GetPBRightEdge();
	float p1Left = m_p1->GetPBLeftEdge();
	// プレイヤー2
	float p2Right = m_p2->GetPBRightEdge();
	float p2Left = m_p2->GetPBLeftEdge();

	/*押し戻し条件*/
	//押し戻し判定どうしが当たっているか
	bool isHitPushBack = IsCol1PTo2P(ColType::PushBack, ColType::PushBack).isCol;
	// 地上(Player1)
	bool p1IsLand = m_p1->GetIsLand();
	// 地上(Player2)
	bool p2IsLand = m_p2->GetIsLand();
	// 落下中(Player1)
	bool p1Fall = m_p1->GetMoveY() > 0;
	// 落下中(Player1)
	bool p2Fall = m_p2->GetMoveY() > 0;
	//両者空中
	bool isBothPlayerJump = !p1IsLand && !p2IsLand;
	//どちらか空中(どっちもtrueどっちもfalseはダメ)
	bool isOneSidePlayerJump = p1IsLand xor p2IsLand;
	//両者地上
	bool isBothPlayerLand = p1IsLand && p2IsLand;
	// どちらかが落下中(どっちもtrueどっちもfalseはダメ)
	bool isOneSidePlayerFall = p1Fall xor p2Fall;
	// 両方とも画面端
	bool isBothPlayerEdge = p1Edge && p2Edge;

	//押し戻しが当たってない
	if (!isHitPushBack)
	{
		m_p1->SetIsPushBack(PushBackType::None);
		m_p2->SetIsPushBack(PushBackType::None);
		return;//早期リターン
	}
	//どちらも空中での押し戻し
	else if (isHitPushBack && isBothPlayerJump)
	{
		m_p1->SetIsPushBack(PushBackType::Balance);
		m_p2->SetIsPushBack(PushBackType::Balance);
	}
	//どちらかが空中にいるときの押し戻し
	else if (isHitPushBack && isOneSidePlayerJump && (p1Fall || p2Fall))
	{
		// P1とP2のめり込みを計算
			// P1左側、P2右側の場合
		if (p1Pos.x <= p2Pos.x)
		{
			// めり込み量
			float overlap = p1Right - p2Left;
			// P1は左側へ
			m_p1->SetPos({ m_p1->GetPos().x - overlap / 2, m_p1->GetPos().y });
			// P2は右側へ
			m_p2->SetPos({ m_p2->GetPos().x + overlap / 2, m_p2->GetPos().y });

		}
		// P2左側、P1右側の場合
		else if (p1Pos.x >= p2Pos.x)
		{
			float overlap = p2Right - p1Left;
			// P1は右側へ
			m_p1->SetPos({ m_p1->GetPos().x + overlap / 2, m_p1->GetPos().y });
			// P2は左側へ
			m_p2->SetPos({ m_p2->GetPos().x - overlap / 2, m_p2->GetPos().y });
		}
	}

	//地上での押し戻し
	else if (isHitPushBack && isBothPlayerLand)
	{
		// プレイヤー1,2の動きから押し戻しによる動きを決定する
		// 動きなし
		if ((p1Move == 0 && p2Move == 0) || (p1Move > 0 && p2Move > 0) ||
			(p1Move > 0 && p2Edge) || (p2Move > 0 && p1Edge))
		{
			m_p1->SetIsPushBack(PushBackType::Balance);
			m_p2->SetIsPushBack(PushBackType::Balance);
		}
		// プレイヤー１がプレイヤー２を押す
		else if (p1Move > 0 && p2Move == 0)
		{
			m_p1->SetIsPushBack(PushBackType::Push);
			m_p2->SetIsPushBack(PushBackType::Pushed);
		}
		// プレイヤー２がプレイヤー１を押す
		else if (p1Move == 0 && p2Move > 0)
		{
			m_p1->SetIsPushBack(PushBackType::Pushed);
			m_p2->SetIsPushBack(PushBackType::Push);
		}
		// プレイヤー１がプレイヤー２の後ろ方向の動きより大きいとき
		else if ((p2Move < 0) && (p1Move > -p2Move))
		{
			m_p1->SetIsPushBack(PushBackType::Balance);
			m_p2->SetIsPushBack(PushBackType::None);
		}
		// プレイヤー１がプレイヤー２の後ろ方向の動きより大きいとき
		else if ((p1Move < 0) && (p2Move > -p1Move))
		{
			m_p1->SetIsPushBack(PushBackType::None);
			m_p2->SetIsPushBack(PushBackType::Balance);
		}
		// それ以外のパターン（押し合い判定から離れるとき）
		else
		{
			m_p1->SetIsPushBack(PushBackType::None);
			m_p2->SetIsPushBack(PushBackType::None);
		}
	}
	/*
	// 空中での押し戻し
	else if (!p1IsLand || !p2IsLand)
	{
		if (p1Fall || p2Fall)
		{
			// P1とP2のめり込みを計算
			// P1左側、P2右側の場合
			if (p1Pos.x <= p2Pos.x)
			{
				// めり込み量
				float overlap = p1Right - p2Left;
				// P1は左側へ
				m_p1->SetPos({ m_p1->GetPos().x - overlap / 2, m_p1->GetPos().y });
				// P2は右側へ
				m_p2->SetPos({ m_p2->GetPos().x + overlap / 2, m_p2->GetPos().y });

			}
			// P2左側、P1右側の場合
			else if (p1Pos.x >= p2Pos.x)
			{
				float overlap = p2Right - p1Left;
				// P1は右側へ
				m_p1->SetPos({ m_p1->GetPos().x + overlap / 2, m_p1->GetPos().y });
				// P2は左側へ
				m_p2->SetPos({ m_p2->GetPos().x - overlap / 2, m_p2->GetPos().y });
			}
		}
	}

	// どちらも地上にいる場合
	if (p1IsLand && p2IsLand && isHitPushBack)
	{
		// プレイヤー1,2の動きから押し戻しによる動きを決定する
		// 動きなし
		if ((p1Move == 0 && p2Move == 0) || (p1Move > 0 && p2Move > 0) ||
			(p1Move > 0 && p2Edge) || (p2Move > 0 && p1Edge))
		{
			m_p1->SetIsPushBack(PushBackType::Balance);
			m_p2->SetIsPushBack(PushBackType::Balance);
		}
		// プレイヤー１がプレイヤー２を押す
		else if (p1Move > 0 && p2Move == 0)
		{
			m_p1->SetIsPushBack(PushBackType::Push);
			m_p2->SetIsPushBack(PushBackType::Pushed);
		}
		// プレイヤー２がプレイヤー１を押す
		else if (p1Move == 0 && p2Move > 0)
		{
			m_p1->SetIsPushBack(PushBackType::Pushed);
			m_p2->SetIsPushBack(PushBackType::Push);
		}
		// プレイヤー１がプレイヤー２の後ろ方向の動きより大きいとき
		else if ((p2Move < 0) && (p1Move > -p2Move))
		{
			m_p1->SetIsPushBack(PushBackType::Balance);
			m_p2->SetIsPushBack(PushBackType::None);
		}
		// プレイヤー１がプレイヤー２の後ろ方向の動きより大きいとき
		else if ((p1Move < 0) && (p2Move > -p1Move))
		{
			m_p1->SetIsPushBack(PushBackType::None);
			m_p2->SetIsPushBack(PushBackType::Balance);
		}
		// それ以外のパターン（押し合い判定から離れるとき）
		else
		{
			m_p1->SetIsPushBack(PushBackType::None);
			m_p2->SetIsPushBack(PushBackType::None);
		}
	}*/

}

IsColData ColCheck::IsCol1PTo2P(ColType _type1, ColType _type2)
{
	IsColData data = {};
	std::vector<Rect> p1 = m_p1->GetCol(_type1);
	std::vector<Rect> p2 = m_p2->GetCol(_type2);
	for (int i = 0; i < p1.size(); i++)
	{
		for (int j = 0; j < p2.size(); j++)
		{
			if (p1[i].IsCollision(p2[j]))
			{
				data.colPos = p1[i].GetColCenterPos(p2[j]);
				data.isCol = true;
				return data;
			}
		}
	}
	// 当たっていないとき
	return { false, {0, 0} };
}

IsColData ColCheck::IsCol2PTo1P(ColType _type1, ColType _type2)
{
	IsColData data = {};
	std::vector<Rect> p1 = m_p1->GetCol(_type1);
	std::vector<Rect> p2 = m_p2->GetCol(_type2);
	for (int i = 0; i < p2.size(); i++)
	{
		for (int j = 0; j < p1.size(); j++)
		{
			if (p2[i].IsCollision(p1[j]))
			{
				data.colPos = p2[i].GetColCenterPos(p1[j]);
				data.isCol = true;
				return data;
			}
		}
	}
	// 当たっていないとき
	return { false, {0, 0} };
}
