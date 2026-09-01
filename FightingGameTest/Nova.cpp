#include "Nova.h"

namespace
{
	constexpr int kGraficSize = 480;

	// 速度
	constexpr float kSpeed = 4;
	constexpr float kPushBackSpeed = 1.5f;
	constexpr float kTatsuSpeed = 1.4f;
	constexpr float kDashSpeed = 3.0f;
	constexpr float kRollSpeed = 2.0f;
	constexpr float kJumpRollSpeedX = 3.0f;
	constexpr float kJumpRollSpeedY = 10.0f;
	// ダメージ
	/*constexpr float kLDamageSpeed = -0.7f;
	constexpr float kMDamageSpeed = -1.5f;
	constexpr float kSDamageSpeed = -0.2f;*/
	constexpr float kKnockBackSpeed = -1.0f;
	constexpr float kDownKnockBackSpeed = -1.5f;
	constexpr float kDownKnockBackAirSpeed = -2.0f;
	constexpr float kGuardBackSpeed = -1.0f;
	constexpr float kLastDamagedSpeed = -2.0f;

	// Y方向の速度
	constexpr float kLastDamagedPowerY = -20.0f;

	// クールダウン
	// アニメーションが終了してからの時間
	// 移動関連
	constexpr int kDashCD = 2;
	// 攻撃関連
	constexpr int kPunchCD = 5;
	constexpr int kKickCD = 15;
	constexpr int kRollCD = 20;
	constexpr int kHadouCD = 7;
	constexpr int kTatsuCD = 17;

	// ジャンプ
	constexpr float kJumpSpeed = -35.0f;
	constexpr float kJumpSpeedX = 1.7f;
	constexpr float kGravityAccel = 1.2f;
	constexpr float kFallGravityScale = 1.5f;
	constexpr float kGroundPos = Game::kScreenHeight * 0.4f;

	// アニメーションを遷移を行う空中スピード
	constexpr float kJumpingToTopSpeed = -5.0f;
	constexpr float kTopToFallingSpeed = 3.0f;

	// HPに関する処理
	constexpr int kMaxHp = 1000;

	// 技のダメージを設定
	constexpr int kPunchDamage = 100;
	constexpr int kKickDamage = 100;
	constexpr int kTatsuDamage = 200;
	constexpr int kRollDamage = 100;
	constexpr int kJumpPunchDamage = 100;
	constexpr int kJumpKickDamage = 100;

	// 当たり判定のBoxを設定する
	//体
	constexpr Box kBodyBox = { kGraficSize * 0.375f, kGraficSize * 0.425f, kGraficSize * 0.25f, kGraficSize * 0.325f };
	constexpr Box kBodyDashForwardBox = { kBodyBox.x, kBodyBox.y, kBodyBox.width, kBodyBox.height };
	constexpr Box kBodyCrouchBox = { kBodyBox.x, kGraficSize * 0.7f, kBodyBox.width, kBodyBox.height };
	constexpr Box kBodyPunchBox = { kGraficSize * 0.5f, kGraficSize * 0.45f, kGraficSize * 0.2f, kGraficSize * 0.25f };
	constexpr Box kBodyKickBox = { kBodyBox.x, kBodyBox.y, kBodyBox.width, kGraficSize * 0.55f };
	constexpr Box kBodyRollBox = { kGraficSize * 0.325f, kGraficSize * 0.325f, kGraficSize * 0.35f, kGraficSize * 0.35f };
	constexpr Box kBodyKickBottomBox = { kGraficSize * 0.35f, kGraficSize * 0.8f, kGraficSize * 0.25f, kGraficSize * 0.2f };
	constexpr Box kBodyTatsuBox = { kGraficSize * 0.35f, kGraficSize * 0.35f, kGraficSize * 0.4f, kGraficSize * 0.65f };
	// 頭
	constexpr Box kHeadBox = { kGraficSize * 0.4125f, kGraficSize * 0.2f, kGraficSize * 0.175f, kGraficSize * 0.225f };
	constexpr Box kHeadDashForwardBox = { kGraficSize * 0.6f, kGraficSize * 0.45f, kGraficSize * 0.2f, kGraficSize * 0.2f };
	constexpr Box kHeadCrouchBox = { kGraficSize * 0.45f, kGraficSize * 0.45f, kGraficSize * 0.25f, kGraficSize * 0.3f };
	constexpr Box kHeadPunchBox = { kGraficSize * 0.6f, kGraficSize * 0.25f, kGraficSize * 0.2f, kGraficSize * 0.2f };
	constexpr Box kHeadKickBox = { kGraficSize * 0.2f, kGraficSize * 0.5f, kGraficSize * 0.2f, kGraficSize * 0.2f };
	constexpr Box kHeadKickBottomBox = { kGraficSize * 0.2f, kGraficSize * 0.6f, kGraficSize * 0.2f, kGraficSize * 0.4f };
	constexpr Box kHeadTatsuBox = { kGraficSize * 0.5f, kGraficSize * 0.5f, kGraficSize * 0.2f, kGraficSize * 0.2f };
	// 足
	constexpr Box kLegBox = { kGraficSize * 0.325f, kGraficSize * 0.6f, kGraficSize * 0.35f, kGraficSize * 0.4f };
	constexpr Box kLegDashForwardBox = { kGraficSize * 0.1f, kGraficSize * 0.45f, kGraficSize * 0.35f, kGraficSize * 0.3f };
	constexpr Box kLegCrouchBox = { kGraficSize * 0.375f, kGraficSize * 0.7f, kGraficSize * 0.25f, kGraficSize * 0.3f }; // ここゼロ
	constexpr Box kLegWalkBox = { kGraficSize * 0.375f, kGraficSize * 0.6f, kGraficSize * 0.25f, kGraficSize * 0.4f };
	constexpr Box kLegKickBox = { kGraficSize * 0.6f, kGraficSize * 0.3f, kGraficSize * 0.35f, kGraficSize * 0.2f };
	constexpr Box kLegKickBottomBox = { kGraficSize * 0.53f, kGraficSize * 0.85f, kGraficSize * 0.5f, kGraficSize * 0.15f };
	constexpr Box kLegTatsuBox = { kGraficSize * 0.5f, kGraficSize * 0.5f, kGraficSize * 0, kGraficSize * 0 };
	// 腕
	constexpr Box kArmBox = { kGraficSize * 0.375f, kGraficSize * 0.45f, kGraficSize * 0, kGraficSize * 0 }; // ここゼロ
	constexpr Box kArmDashForwardBox = { kGraficSize * 0.375f, kGraficSize * 0.45f, kGraficSize * 0, kGraficSize * 0 }; // ここゼロ
	constexpr Box kArmCrouchBox = { kGraficSize * 0.375f, kGraficSize * 0.7f, kGraficSize * 0.25f, kGraficSize * 0.3f }; // ここゼロ
	constexpr Box kArmPunchBox = { kGraficSize * 0.8f, kGraficSize * 0.25f, kGraficSize * 0.1f, kGraficSize * 0.175f };
	constexpr Box kArmKickBottomBox = { kGraficSize * 0.35f, kGraficSize * 0.8f, kGraficSize * 0, kGraficSize * 0 };
	constexpr Box kArmTatsuBox = { kGraficSize * 0.5f, kGraficSize * 0.5f, kGraficSize * 0, kGraficSize * 0 };

	// 押し戻し
	constexpr Box kPBBox = { kGraficSize * 0.4f, kGraficSize * 0.3f, kGraficSize * 0.2f, kGraficSize * 0.7f };

	//攻撃関連
	//上段
	constexpr Box kSPunchBox = { kGraficSize * 0.75f, kGraficSize * 0.2f, kGraficSize * 0.15f, kGraficSize * 0.2f };
	constexpr Box kKickBox = { kGraficSize * 0.6f, kGraficSize * 0.3f, kGraficSize * 0.35f, kGraficSize * 0.15f };
	constexpr Box kTatsuBox0 = { kGraficSize * 0.2f, kGraficSize * 0.2f, kGraficSize * 0.35f, kGraficSize * 0.2f };
	constexpr Box kTatsuBox1 = { kGraficSize * 0.2f, kGraficSize * 0.2f, kGraficSize * 0.7f, kGraficSize * 0.2f };
	constexpr Box kTatsuBox2 = { kGraficSize * 0.4f, kGraficSize * 0.2f, kGraficSize * 0.5f, kGraficSize * 0.6f };
	constexpr Box kTatsuBox3 = { kGraficSize * 0.6f, kGraficSize * 0.2f, kGraficSize * 0.35f, kGraficSize * 0.5f };
	constexpr Box kRollBox = { kGraficSize * 0.3f, kGraficSize * 0.3f, kGraficSize * 0.4f, kGraficSize * 0.4f };
	//中段
	constexpr Box kJumpPunchBox = { kGraficSize * 0.63f, kGraficSize * 0.5f, kGraficSize * 0.2f, kGraficSize * 0.2f };
	constexpr Box kJumpKickBox = { kGraficSize * 0.63f, kGraficSize * 0.3f, kGraficSize * 0.35f, kGraficSize * 0.25f };
	// 下段
	constexpr Box kKickBottomBox = { kGraficSize * 0.63f, kGraficSize * 0.8f, kGraficSize * 0.4f, kGraficSize * 0.2f };
}

void Nova::SetAnimData()
{
	// アニメーションデータの定義リスト
	// アニメーション追加時触るのはここだけ
	const std::map<AnimID, AnimData> kAnimConfig =
	{
		// {データID、{ハンドル、1コマの表示フレーム、全体コマ数、ループするかどうか}}を示す
		// アイドル状態
		{AnimID::Idle, {"anim/idle.png", 10, 7, true}},
		// ジャンプ状態
		{AnimID::JumpFront, {"anim/jumpFront.png", 10, 2, false}},
		{AnimID::Jumping, {"anim/jumping.png", 10, 1, false}},
		{AnimID::JumpForward, {"anim/jumpForward.png", 15, 2, false}},
		{AnimID::JumpBack, {"anim/jumpBack.png", 10, 2, false}},
		{AnimID::JumpTop, {"anim/jumpTop.png", 10, 1, false}},
		{AnimID::JumpFalling, {"anim/falling.png", 8, 3, false}},
		{AnimID::JumpLanding, {"anim/landing.png", 5, 1, false}},
		// 移動状態
		{AnimID::WalkForward, {"anim/walkForward.png", 10, 6, true}},
		{AnimID::DashForward, {"anim/supfo.png", 5, 5, false}},
		{AnimID::WalkBack, {"anim/walkBack.png", 10, 6, true}},
		// しゃがみ状態
		{AnimID::Crouch, {"anim/crouch.png", 15, 3, true}},
		// ガード状態
		{AnimID::GuardTop, {"anim/guardTop.png", 20, 1, false}},
		// 攻撃状態
		{AnimID::Punch, {"anim/sPunch.png", 5, 3, false}},
		{AnimID::Kick, {"anim/sKick.png", 5, 3, false}},
		{AnimID::KickBottom, {"anim/slide.png", 5, 3, false}},
		// コマンドの攻撃
		{AnimID::Hadou, {"anim/Hadou.png", 8, 5, false}},
		{AnimID::Tatsumaki, {"anim/dKick.png", 8, 4, false}},
		{AnimID::Roll, {"anim/rolling.png", 5, 8, false}},
		// 空中攻撃の状態
		{AnimID::JumpPunch, {"anim/jumpPunch.png", 5, 3, false}},
		{AnimID::JumpKick, {"anim/jumpKick.png", 8, 2, false}},
		// ダメージを受けた状態
		{AnimID::sDamaged, {"anim/sDamaged.png", 5, 3, false}},
		{AnimID::Damaged, {"anim/damaged.png", 5, 4, false}},
		{AnimID::LLDamaged, {"anim/LLDamaged.png", 7, 7}},
		{AnimID::AirDamaged, {"anim/airDamaged.png", 7, 3, false}},
		{AnimID::LastDamaged, {"anim/LLDamaged.png", 5, 7, false}},
		{AnimID::DownDamaged, {"anim/downDamaged.png", 7, 8, false}}
	};

	// データをもとにAnimationクラスを生成してマップに登録
	for (const auto& [id, data] : kAnimConfig)
	{
		int handle = LoadGraph(data.filePath);
		// マップ内に新しくAnimationを作り、Initを呼ぶ
		m_animations[id].Init(handle, data.animFrame, data.animNum, data.isLoop);
	}
}

void Nova::SetColData()
{
	// 当たり判定の初期化
	for (int i = 0; i < static_cast<int>(ColType::Max); i++)
	{
		for (int j = 0; j < static_cast<int>(ColDetail::Max); j++)
		{
			m_colmap[i][j].box = {};
			m_colmap[i][j].active = false;
		}
	}

	// 当たり判定の設定（必要なものだけ設定していく）
	// Boxの設定、色の設定、行動との結びつけ
	// やられ
	// ボディ
	SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, kLegBox, kArmBox);
	SetBoxBodyHead(ColType::Invisible, kBodyBox, kHeadBox, kLegBox, kArmBox);
	SetBoxBodyHead(ColType::GuardTop, kBodyBox, kHeadBox, kLegBox, kArmBox);
	SetBoxBodyHead(ColType::GuardBottom, kBodyBox, kHeadBox, kLegBox, kArmBox);
	m_colmap[static_cast<int>(ColType::Hurt)][static_cast<int>(ColDetail::Body)].active = true;
	m_colmap[static_cast<int>(ColType::Hurt)][static_cast<int>(ColDetail::Head)].active = true;
	m_colmap[static_cast<int>(ColType::Hurt)][static_cast<int>(ColDetail::Leg)].active = true;
	m_colmap[static_cast<int>(ColType::Hurt)][static_cast<int>(ColDetail::Arm)].active = true;

	// 押し戻し
	SetColBox(ColType::PushBack, ColDetail::Body, kPBBox);
	m_colmap[static_cast<int>(ColType::PushBack)][static_cast<int>(ColDetail::Body)].active = true;

	// 攻撃
	// 上段
	SetColBox(ColType::HitTop, ColDetail::Arm, kSPunchBox);
	SetColBox(ColType::HitTop, ColDetail::Leg, kKickBox);
	SetColBox(ColType::HitTop, ColDetail::Leg, kTatsuBox0);
	SetColBox(ColType::HitTop, ColDetail::Body, kRollBox);

	// 中段
	SetColBox(ColType::HitMiddle, ColDetail::Arm, kJumpPunchBox);
	SetColBox(ColType::HitMiddle, ColDetail::Leg, kJumpKickBox);

	// 下段
	SetColBox(ColType::HitBottom, ColDetail::Leg, kKickBottomBox);

	ChangeColType(ColType::Hurt);
}

void Nova::SetCurrentAction(StateID _state)
{
	// 描画する画像のサイズ
	m_graficSize = kGraficSize;

	// アニメーションと動きを設定
	switch (_state)
	{
	case StateID::Idle:
		ChangeAnim(AnimID::Idle);
		ChangeColType(ColType::Hurt);
		SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, kLegBox, kArmBox);
		SetActionInfo(0, 0, 0, false, false);
		break;

		// ジャンプの状態
	case StateID::JumpNeutral:
		ChangeAnim(AnimID::Crouch);
		ChangeColType(ColType::Hurt);
		SetActionInfo(0, kJumpSpeed, 0, false, false);
		SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, {}, kArmBox);
		break;

	case StateID::JumpForward:
		ChangeAnim(AnimID::Crouch);
		ChangeColType(ColType::Hurt);
		SetActionInfo(m_speed * kJumpSpeedX, kJumpSpeed, 0, false, false);
		SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, {}, kArmBox);
		break;

	case StateID::JumpBack:
		ChangeAnim(AnimID::Crouch);
		ChangeColType(ColType::Hurt);
		SetActionInfo(-m_speed * kJumpSpeedX, kJumpSpeed, 0, false, false);
		SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, {}, kArmBox);
		break;

		// 移動の状態
	case StateID::WalkForward:
		ChangeAnim(AnimID::WalkForward);
		ChangeColType(ColType::Hurt);
		SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, kLegWalkBox, kArmBox);
		SetActionInfo(m_speed, 0, 0, false, false);
		break;

	case StateID::DashForward:
		ChangeAnim(AnimID::DashForward);
		ChangeColType(ColType::Hurt);
		SetActionInfo(m_speed * kDashSpeed, 0, kDashCD, true, true);
		SetBoxBodyHead(ColType::Hurt, kBodyDashForwardBox, kHeadDashForwardBox, kLegDashForwardBox, kArmDashForwardBox);
		break;

	case StateID::WalkBack:
		ChangeAnim(AnimID::WalkBack);
		ChangeColType(ColType::GuardTop);
		SetBoxBodyHead(ColType::GuardTop, kBodyBox, kHeadBox, kLegWalkBox, kArmBox);
		SetActionInfo(-m_speed, 0, 0, false, false);
		break;

	case StateID::DashBack:
		ChangeAnim(AnimID::DashForward);
		ChangeColType(ColType::Hurt);
		SetActionInfo(-m_speed * kDashSpeed, 0, kDashCD, true, true);
		break;

		// しゃがみの状態
	case StateID::Crouch:
		ChangeAnim(AnimID::Crouch);
		ChangeColType(ColType::Hurt);
		SetBoxBodyHead(ColType::Hurt, kBodyCrouchBox, kHeadCrouchBox, kLegCrouchBox, kArmCrouchBox);
		SetActionInfo(0, 0, 0, false, false);
		break;

	case StateID::CrouchBack:
		ChangeAnim(AnimID::Crouch);
		ChangeColType(ColType::GuardBottom);
		SetBoxBodyHead(ColType::GuardBottom, kBodyCrouchBox, kHeadCrouchBox, kLegCrouchBox, kArmCrouchBox);
		SetActionInfo(0, 0, 0, false, false);
		break;

		// ガードの状態
	case StateID::GuardTop:
		ChangeAnim(AnimID::GuardTop);
		ChangeColType(ColType::GuardTop);
		// 仮 ガードした後動きが欲しいノックバック
		SetActionInfo(m_speed * kGuardBackSpeed, 0, 0, true, false);
		m_damaged = true;
		break;

	case StateID::GuardBottom:
		ChangeAnim(AnimID::GuardTop);
		ChangeColType(ColType::GuardBottom);
		SetActionInfo(m_speed * kGuardBackSpeed, 0, 0, true, false);
		m_damaged = true;
		break;

		// 攻撃の状態
	case StateID::Punch:
		// ヒットバックのある攻撃
		m_hitBackAction = true;
		ChangeAnim(AnimID::Punch);
		ChangeColType(ColType::Hurt);
		SetActionInfo(0, 0, kPunchCD, true, false);
		SetBoxBodyHead(ColType::Hurt, kBodyPunchBox, kHeadPunchBox, kLegBox, kArmPunchBox);
		m_mainDamage = kPunchDamage;
		m_attackScale = AttackType::S;
		m_isAttackState = true;
		break;

	case StateID::Kick:
		// ヒットバックのある攻撃
		m_hitBackAction = true;
		ChangeAnim(AnimID::Kick);
		ChangeColType(ColType::Hurt);
		SetActionInfo(0, 0, kKickCD, true, true);
		SetBoxBodyHead(ColType::Hurt, kBodyKickBox, kHeadKickBox, kLegKickBox, kArmBox);
		m_mainDamage = kKickDamage;
		m_attackScale = AttackType::M;
		m_isAttackState = true;
		break;

	case StateID::KickBottom:
		// ヒットバックのある攻撃
		m_hitBackAction = true;
		ChangeAnim(AnimID::KickBottom);
		ChangeColType(ColType::Hurt);
		SetActionInfo(0, 0, kKickCD, true, false);
		SetBoxBodyHead(ColType::Hurt, kBodyKickBottomBox, kHeadKickBottomBox, kLegKickBottomBox, kArmKickBottomBox);
		m_mainDamage = kKickDamage;
		m_attackScale = AttackType::L;
		m_isAttackState = true;
		break;

		// コマンドの状態
	case StateID::Hadou:
		ChangeAnim(AnimID::Hadou);
		ChangeColType(ColType::Hurt);
		SetActionInfo(0, 0, kHadouCD, true, false);
		break;

	case StateID::Tatsumaki:
		ChangeAnim(AnimID::Tatsumaki);
		ChangeColType(ColType::Hurt);
		SetActionInfo(m_speed * kTatsuSpeed, 0, kTatsuCD, true, true);
		SetBoxBodyHead(ColType::Hurt, kBodyTatsuBox, kHeadTatsuBox, kLegTatsuBox, kArmTatsuBox);
		m_mainDamage = kTatsuDamage;
		m_isAttackState = true;
		break;

	case StateID::Roll:
		ChangeAnim(AnimID::Roll);
		ChangeColType(ColType::Hurt);
		SetBoxBodyHead(ColType::Hurt, kBodyRollBox, kBodyRollBox, kLegBox, kArmBox);
		SetActionInfo(m_speed * kRollSpeed, 0, kRollCD, true, false);
		m_mainDamage = kRollDamage;
		m_isAttackState = true;
		break;

		// 空中攻撃の状態
	case StateID::JumpPunch:
		m_isJumpAttack = true;
		ChangeAnim(AnimID::JumpPunch);
		ChangeColType(ColType::Hurt);
		SetActionInfo(m_virtualMove.x, m_virtualMove.y, 0, true, false);
		m_mainDamage = kJumpPunchDamage;
		m_attackScale = AttackType::S;
		m_isAttackState = true;
		break;

	case StateID::JumpKick:
		m_isJumpAttack = true;
		ChangeAnim(AnimID::JumpKick);
		ChangeColType(ColType::Hurt);
		SetActionInfo(m_virtualMove.x, m_virtualMove.y, 0, true, false);
		m_mainDamage = kJumpKickDamage;
		m_attackScale = AttackType::S;
		m_isAttackState = true;
		break;

	case StateID::JumpRoll:
		m_isJumpAttack = true;
		m_offGravity = true;
		ChangeAnim(AnimID::Roll);
		ChangeColType(ColType::Hurt);
		SetBoxBodyHead(ColType::Hurt, kBodyRollBox, kBodyRollBox, kLegBox, kArmBox);
		SetActionInfo(m_speed * kJumpRollSpeedX, kJumpRollSpeedY, 0, true, false);
		m_mainDamage = kRollDamage;
		break;

	case StateID::Damaged:
		// 転ぶ
		// 倒されたときのアニメーションは決まったものとする
		if (m_isLastDamaged)
		{
			// 倒されたときのアニメーションは決まったものとする
			ChangeAnim(AnimID::LastDamaged);
			SetActionInfo(m_speed * kLastDamagedSpeed, kLastDamagedPowerY, 100, true, false);
			ChangeColType(ColType::Invisible);
		}
		else if (m_damagedScale == AttackType::Down)
		{
			ChangeAnim(AnimID::DownDamaged);
			SetActionInfo(m_speed * kDownKnockBackSpeed, 0, 30, true, false);
			ChangeColType(ColType::Invisible);
		}
		else if (m_damagedScale == AttackType::L)
		{
			ChangeAnim(AnimID::Damaged);
			SetActionInfo(m_speed * kKnockBackSpeed, 0, 20, true, false);
			ChangeColType(ColType::Invisible);
		}
		// 吹き飛ぶ
		else if (m_damagedScale == AttackType::M)
		{
			ChangeAnim(AnimID::AirDamaged);
			SetActionInfo(m_speed * kKnockBackSpeed, 0, 20, true, false);
			ChangeColType(ColType::Invisible);
		}
		// ちょっと食らう
		else
		{
			ChangeAnim(AnimID::sDamaged);
			SetActionInfo(m_speed * kKnockBackSpeed, 0, 20, true, false);
			ChangeColType(ColType::Hurt);
		}
		//SetActionInfo(-m_speed, 0, 20, true, false);
		m_damaged = true;
		ResetAttackCol();
		break;

	case StateID::AirDamaged:
		if (m_isLastDamaged)
		{
			// 倒されたときのアニメーションは決まったものとする
			ChangeAnim(AnimID::LastDamaged);
			SetActionInfo(m_speed * kLastDamagedSpeed, -20, 100, true, false);
			ChangeColType(ColType::Invisible);
		}
		else if (m_damagedScale == AttackType::Down)
		{
			m_isDownDamaged = true;
			ChangeAnim(AnimID::DownDamaged);
			SetActionInfo(m_speed * kDownKnockBackAirSpeed, -30, 30, true, false);
			SetBoxBodyHead(ColType::Invisible, kBodyBox, kHeadBox, kLegBox, kArmBox);
			ChangeColType(ColType::Invisible);
		}
		else
		{
			ChangeAnim(AnimID::AirDamaged);
			ChangeColType(ColType::Hurt);
			SetActionInfo(-m_speed * 2, -20, 20, true, false);
		}
		m_damaged = true;
		m_offGravity = false;
		ResetAttackCol();
		break;

	case StateID::None:

		if (m_isAction && m_pCurrentAnim->IsEnd())
		{
			if (m_coolDownTimer <= 0 || m_cdAnim)
			{
				m_isAction = false;

				// 変数の初期化
				m_isHadou = false;
				m_isPunch = false;
				m_damaged = false;
				m_isHitOnce = false;
				m_isHitBack = false;
				m_isDownDamaged = false;

				// 当たり判定を標準に戻す
				// 当たり判定を標準（Idle）に戻す
				SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, kLegBox, kArmBox);
				SetBoxBodyHead(ColType::Invisible, kBodyBox, kHeadBox, kLegBox, kArmBox);
				SetBoxBodyHead(ColType::GuardTop, kBodyBox, kHeadBox, kLegBox, kArmBox);

				if (m_isLanding)
				{
					ChangeColType(ColType::Hurt);
					if (m_cdAnim)
					{
						ChangeAnim(AnimID::Idle);
						m_cdAnim = false;
					}
					else
					{
						// アニメーションのクールダウン中バグるためけしてる
						//m_pCurrentAnim->Reset();
						m_changeSameAnim = true;
					}
				}
			}
		}
		break;
	}
}

void Nova::SetActionDetail()
{
	// 細かいところ（フレーム指定で行う処理）
	// エフェクトの処理
	if (m_currentAnimID == AnimID::Hadou)
	{
		// 指定されたフレームでのみ発動
		if (m_pCurrentAnim->RequestAnimIdxAndFrameRange(2, 2, 0, 15))
		{
			m_isHadou = true;
		}
		else
		{
			m_isHadou = false;
		}
	}
	// アニメーション基準で動きを調整する
	else if (m_currentAnimID == AnimID::Damaged)
	{
		// ダメージの3コマ目で動きを止める
		if (m_pCurrentAnim->RequestAnimIdxAndFrameRange(3, 3, 0, 0))
		{
			m_virtualMove.x = 0;
		}
	}
	else if (m_currentAnimID == AnimID::GuardTop)
	{
		if (m_pCurrentAnim->RequestAnimIdxAndFrameRange(0, 0, 10, 20))
		{
			m_virtualMove.x = 0;
		}
	}
	else if (m_currentAnimID == AnimID::Tatsumaki)
	{
		if (m_pCurrentAnim->RequestAnimIdxAndFrameRange(3, 3, 7, 8))
		{
			m_virtualMove.x = 0;
		}
	}
	else if (m_currentAnimID == AnimID::DownDamaged)
	{
		if (m_pCurrentAnim->RequestAnimIdxAndFrameRange(7, 7, 0, 5))
		{
			m_virtualMove.x = 0;
		}
	}
}

void Nova::SetAttackCol()
{
	switch (m_currentAnimID)
	{
	case AnimID::Punch:
		// 指定したフレームの間コリジョンを生成する
		GeneCol(ColType::HitTop, ColDetail::Arm, 1, 1, 0, 5);
		break;

	case AnimID::Kick:
		// 足の上段攻撃のコリジョンを設定する（竜巻に上書きされるから）
		SetColBox(ColType::HitTop, ColDetail::Leg, kKickBox);
		GeneCol(ColType::HitTop, ColDetail::Leg, 2, 2, 0, 5);
		break;

	case AnimID::KickBottom:
		GeneCol(ColType::HitBottom, ColDetail::Leg, 2, 2, 0, 5);
		break;

	case AnimID::Tatsumaki:
		GeneCol(ColType::HitTop, ColDetail::Leg, 0, 3, 5, 5);
		// 攻撃の強度を最終段のみMにする
		if (m_pCurrentAnim->RequestAnimIdx(3))
		{
			m_attackScale = AttackType::M;
		}
		else
		{
			m_attackScale = AttackType::S;
		}
		// フレームによって技の当たり判定を変化させる
		SetFrameColBox(ColType::HitTop, ColDetail::Leg, 0, 0, kTatsuBox0);
		SetFrameColBox(ColType::HitTop, ColDetail::Leg, 1, 0, kTatsuBox1);
		SetFrameColBox(ColType::HitTop, ColDetail::Leg, 2, 0, kTatsuBox2);
		SetFrameColBox(ColType::HitTop, ColDetail::Leg, 3, 0, kTatsuBox3);
		break;

	case AnimID::Roll:
		GeneCol(ColType::HitTop, ColDetail::Body, 1, 7, 0, 5);
		break;

	case AnimID::JumpPunch:
		GeneCol(ColType::HitMiddle, ColDetail::Arm, 1, 2, 0, 5);
		break;

	case AnimID::JumpKick:
		GeneCol(ColType::HitMiddle, ColDetail::Leg, 1, 1, 0, 8);
		break;
	}
}

StateID Nova::InputAction(const Pad& _pad)
{
	// インプット
	if (!m_isAction && m_coolDownTimer <= 0 && m_isLanding)
	{
		if (_pad.Trigger(ButtonType::A) || _pad.Trigger(ButtonType::Y))
		{
			// コマンドによる攻撃を優先して取得する
			if (m_commandManager.CheckCommand(m_dir) == "Tatsumaki")
			{
				// 波動拳をするとき
				return StateID::Tatsumaki;
			}
			// 下段キック
			else if (_pad.Press(ButtonType::Down))
			{
				return StateID::KickBottom;
			}
			// コマンドがなければ通常攻撃を出す
			else
			{
				// パンチ
				return StateID::Kick;
			}
		}
		else if (_pad.Trigger(ButtonType::X) || _pad.Trigger(ButtonType::B))
		{
			// コマンドによる攻撃を優先して取得する
			if (m_commandManager.CheckCommand(m_dir) == "Hadou")
			{
				// 波動拳をするとき
				return StateID::Hadou;
			}
			// コマンドがなければ通常攻撃を出す
			else
			{
				// パンチ
				return StateID::Punch;
			}
		}
		else if (_pad.Press(ButtonType::Down))
		{
			// しゃがみ後ろ
			if ((_pad.Press(ButtonType::Left) && m_dir == 1) ||
				(_pad.Press(ButtonType::Right) && m_dir == -1))
			{
				return StateID::CrouchBack;
			}
			// ニュートラルしゃがみ
			else
			{
				return StateID::Crouch;
			}

		}
		else if (_pad.Press(ButtonType::Up))
		{
			// ニュートラルジャンプ
			if (_pad.Press(ButtonType::Right) && _pad.Press(ButtonType::Left))
			{
				return StateID::JumpNeutral;
			}
			// バックジャンプ
			else if ((_pad.Press(ButtonType::Left) && m_dir == 1) ||
				(_pad.Press(ButtonType::Right) && m_dir == -1))
			{
				return StateID::JumpBack;
			}
			// 前ジャンプ
			else if ((_pad.Press(ButtonType::Right) && m_dir == 1) ||
				(_pad.Press(ButtonType::Left) && m_dir == -1))
			{
				return StateID::JumpForward;
			}
			// ニュートラルジャンプ
			else
			{
				return StateID::JumpNeutral;
			}
		}
		else if (_pad.Press(ButtonType::Left) && _pad.Press(ButtonType::Right))
		{
			return StateID::Idle;
		}
		else if ((_pad.Press(ButtonType::Left) && m_dir == 1) ||
			(_pad.Press(ButtonType::Right) && m_dir == -1))
		{
			if (m_commandManager.CheckCommand(m_dir) == "DashBack")
			{
				return StateID::DashBack;
			}
			else
			{
				return StateID::WalkBack;
			}
		}
		else if ((_pad.Press(ButtonType::Right) && m_dir == 1) ||
			(_pad.Press(ButtonType::Left) && m_dir == -1))
		{
			if (m_commandManager.CheckCommand(m_dir) == "DashForward")
			{
				return StateID::DashForward;
			}
			else
			{
				return StateID::WalkForward;
			}
		}
		// いずれのキーも押されていなければ実行
		else if (!_pad.IsPressAnyKey())
		{
			return StateID::Idle;
		}
	}
	// 空中にいるときに行う行動
	else if (!m_isAction && m_coolDownTimer <= 0 && !m_isLanding)
	{
		if (_pad.Trigger(ButtonType::A))
		{
			return StateID::JumpKick;
		}
		else if (_pad.Trigger(ButtonType::X))
		{
			return StateID::JumpPunch;
		}
		else if (_pad.Trigger(ButtonType::B))
		{
			return StateID::JumpRoll;
		}
		else if (_pad.Trigger(ButtonType::Y))
		{

		}
	}
	return StateID::None;
}