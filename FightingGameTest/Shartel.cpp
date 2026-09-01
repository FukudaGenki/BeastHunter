#include "Shartel.h"

namespace
{
	constexpr int kGraficSize = 512;
	constexpr float kGraficDrawAdjY = 30.0f;
	constexpr int kAdjShartelSlashPos = 480;

	// Totsuで使う変数
	constexpr int kTotsuSpeedLv1 = 1;
	constexpr int kTotsuSpeedLv2 = 3;
	constexpr int kTotsuSpeedLv3 = 8;


	// 速度
	constexpr float kSpeed = 4;
	constexpr float kPushBackSpeed = 1.5f;
	constexpr float kTatsuSpeed = 1.4f;
	constexpr float kDashSpeed = 4.0f;
	constexpr float kRollSpeed = 2.0f;
	constexpr float kJumpRollSpeedX = 3.0f;
	constexpr float kJumpRollSpeedY = 10.0f;
	// ダメージを受けた時のノックバック
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
	constexpr int kShoryuDamage = 150;
	constexpr int kTotsuDamageLv1 = 100;
	constexpr int kTotsuDamageLv2 = 150;
	constexpr int kTotsuDamageLv3 = 200;

	// 当たり判定のBoxを設定する
	//体
	constexpr Box kBodyBox = { kGraficSize * 0.4f, kGraficSize * 0.35f, kGraficSize * 0.25f, kGraficSize * 0.35f };
	constexpr Box kBodyCrouchBox = { kBodyBox.x, kGraficSize * 0.525f, kBodyBox.width, kBodyBox.height };
	constexpr Box kBodyPunchBox = kBodyBox;
	constexpr Box kBodyKickBox = kBodyCrouchBox;
	constexpr Box kBodyKickBottomBox = { kGraficSize * 0.35f, kGraficSize * 0.7f, kGraficSize * 0.25f, kGraficSize * 0.2f };
	// 頭
	constexpr Box kHeadBox = { kGraficSize * 0.45f, kGraficSize * 0.15f, kGraficSize * 0.175f, kGraficSize * 0.2f };
	constexpr Box kHeadCrouchBox = { kGraficSize * 0.5f, kGraficSize * 0.325f, kHeadBox.width, kHeadBox.height };
	constexpr Box kHeadPunchBox = kHeadBox;
	constexpr Box kHeadKickBox = kHeadCrouchBox;
	constexpr Box kHeadKickBottomBox = { kGraficSize * 0.2f, kGraficSize * 0.5f, kGraficSize * 0.2f, kGraficSize * 0.4f };
	// 足
	constexpr Box kLegBox = { kGraficSize * 0.35f, kGraficSize * 0.55f, kGraficSize * 0.35f, kGraficSize * 0.4f };
	constexpr Box kLegDashForwardBox = { kGraficSize * 0.1f, kGraficSize * 0.45f, kGraficSize * 0.35f, kGraficSize * 0.3f };
	constexpr Box kLegCrouchBox = { kGraficSize * 0.325f, kGraficSize * 0.7f, kGraficSize * 0.4f, kGraficSize * 0.3f };
	constexpr Box kLegWalkBox = { kGraficSize * 0.375f, kGraficSize * 0.55f, kGraficSize * 0.275f, kGraficSize * 0.4f };
	constexpr Box kLegKickBox = kLegCrouchBox;
	constexpr Box kLegKickBottomBox = { kGraficSize * 0.53f, kGraficSize * 0.75f, kGraficSize * 0.5f, kGraficSize * 0.15f };
	// 腕
	constexpr Box kArmBox = kBodyBox; // ここゼロ
	constexpr Box kArmDashForwardBox = { kGraficSize * 0.375f, kGraficSize * 0.45f, kGraficSize * 0, kGraficSize * 0 }; // ここゼロ
	constexpr Box kArmCrouchBox = kLegCrouchBox; // ここゼロ
	constexpr Box kArmPunchBox = { kGraficSize * 0.625f, kGraficSize * 0.25f, kGraficSize * 0.225f, kGraficSize * 0.175f };
	constexpr Box kArmKickBottomBox = { kGraficSize * 0.35f, kGraficSize * 0.7f, kGraficSize * 0, kGraficSize * 0 };

	// 押し戻し
	constexpr Box kPBBox = { kGraficSize * 0.425f, kGraficSize * 0.25f, kGraficSize * 0.2f, kGraficSize * 0.7f };

	//攻撃関連
	//上段
	constexpr Box kSPunchBox = { kGraficSize * 0.7f, kGraficSize * 0.2f, kGraficSize * 0.15f, kGraficSize * 0.35f };
	constexpr Box kKickBox = { kGraficSize * 0.6f, kGraficSize * 0.5f, kGraficSize * 0.5f, kGraficSize * 0.45f };
	constexpr Box kShoryuBox = { kGraficSize * 0.5f, kGraficSize * 0.3f, kGraficSize * 0.45f, kGraficSize * 0.4f };
	constexpr Box kTotsuBox = { kGraficSize * 0.6f, kGraficSize * 0.5f, kGraficSize * 0.4f, kGraficSize * 0.45f };
	//中段
	constexpr Box kJumpPunchBox = { kGraficSize * 0.63f, kGraficSize * 0.5f, kGraficSize * 0.2f, kGraficSize * 0.2f };
	constexpr Box kJumpKickBox = { kGraficSize * 0.5f, kGraficSize * 0.3f, kGraficSize * 0.45f, kGraficSize * 0.4f };
	// 下段
	constexpr Box kKickBottomBox = { kGraficSize * 0.63f, kGraficSize * 0.7f, kGraficSize * 0.4f, kGraficSize * 0.2f };
}

void Shartel::SetAnimData()
{
	// 描画位置を調整するための変数の設定
	m_graficDrawAdjY = kGraficDrawAdjY;
	// 描画する画像のサイズ
	m_graficSize = kGraficSize;

	// アニメーションデータの定義リスト
	// アニメーション追加時触るのはここだけ
	const std::map<AnimID, AnimData> kAnimConfig =
	{
		// {データID、{ハンドル、1コマの表示フレーム、全体コマ数、ループするかどうか}}を示す
		// アイドル状態
		{AnimID::Idle, {"chara/shartel/idle.png", 10, 9, true}},
		// ジャンプ状態
		{AnimID::JumpFront, {"anim/jumpFront.png", 10, 2, false}},
		{AnimID::Jumping, {"chara/shartel/jumpUp.png", 10, 1, false}},
		{AnimID::JumpForward, {"chara/shartel/jumpForward.png", 15, 1, false}},
		{AnimID::JumpBack, {"chara/shartel/jumpBack.png", 10, 2, false}},
		{AnimID::JumpTop, {"chara/shartel/jumpTop.png", 10, 1, false}},
		{AnimID::JumpFalling, {"chara/shartel/jumpFall.png", 8, 2, false}},
		{AnimID::JumpLanding, {"chara/shartel/landing.png", 5, 1, false}},
		// 移動状態
		{AnimID::WalkForward, {"chara/shartel/walkForward.png", 10, 6, true}},
		{AnimID::DashForward, {"chara/shartel/jumpForward.png", 3, 1, false}},
		{AnimID::WalkBack, {"chara/shartel/walkForward.png", 10, 6, true}},
		// しゃがみ状態
		{AnimID::Crouch, {"chara/shartel/crouch.png", 15, 4, true}},
		// ガード状態
		{AnimID::GuardTop, {"chara/shartel/guardTop.png", 20, 1, false}},
		{AnimID::GuardBottom, {"chara/shartel/guardBottom.png", 20, 1, false}},
		// 攻撃状態
		{AnimID::Punch, {"chara/shartel/sPunch.png", 5, 4, false}},
		{AnimID::Kick, {"chara/shartel/punchBottom.png", 5, 4, false}},
		{AnimID::KickBottom, {"chara/shartel/kickBottom.png", 5, 3, false}},
		// コマンドの攻撃
		{AnimID::Hadou, {"anim/Hadou.png", 8, 5, false}},
		{AnimID::Tatsumaki, {"anim/dKick.png", 8, 4, false}},
		{AnimID::Roll, {"anim/rolling.png", 5, 8, false}},
		{AnimID::Shoryu, {"chara/shartel/shoryuUp.png", 10, 2, false}},
		{AnimID::ShoryuUp, {"chara/shartel/shoryuUp.png", 8, 2, false}},
		{AnimID::ShoryuTop, {"chara/shartel/shoryuTop.png", 8, 1, false}},
		{AnimID::ShoryuFall, {"chara/shartel/shoryuFall.png", 8, 2, false}},
		{AnimID::Kamae, {"chara/shartel/kamae.png", 10, 2, false}},
		{AnimID::Totsu, {"chara/shartel/totsu.png", 5, 5, false}},
		// 空中攻撃の状態
		{AnimID::JumpPunch, {"chara/shartel/jumpPunch.png", 5, 3, false}},
		{AnimID::JumpKick, {"chara/shartel/jumpSlash.png", 8, 2, false}},
		// ダメージを受けた状態
		{AnimID::sDamaged, {"chara/shartel/sDamaged.png", 5, 3, false}},
		{AnimID::Damaged, {"chara/shartel/bottomDamaged.png", 5, 4, false}},
		{AnimID::LLDamaged, {"anim/LLDamaged.png", 7, 7, false}},
		{AnimID::AirDamaged, {"chara/shartel/airDamaged.png", 7, 1, false}},
		{AnimID::LastDamaged, {"chara/shartel/LastDamaged.png", 5, 18, false}},
		{AnimID::DownDamaged, {"chara/shartel/downDamaged.png", 5, 8, false}}

	};

	// データをもとにAnimationクラスを生成してマップに登録
	for (const auto& [id, data] : kAnimConfig)
	{
		int handle = LoadGraph(data.filePath);
		// マップ内に新しくAnimationを作り、Initを呼ぶ
		m_animations[id].Init(handle, data.animFrame, data.animNum, data.isLoop);
	}
}

void Shartel::SetColData()
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
	

	// 中段
	SetColBox(ColType::HitMiddle, ColDetail::Arm, kJumpPunchBox);
	SetColBox(ColType::HitMiddle, ColDetail::Leg, kJumpKickBox);

	// 下段
	SetColBox(ColType::HitBottom, ColDetail::Leg, kKickBottomBox);

	ChangeColType(ColType::Hurt);
}

void Shartel::SetCurrentAction(StateID _state)
{

	// 構え状態を維持する
	if (m_isKamae)
	{
		m_coolDownTimer = 10;
		// 構えをしている時間を数えてそれによってスピードを変える
		m_isKamaeFrame++;
	}
	else if(!m_isKamae && _state != StateID::Totsu)
	{
		// 初期化
		m_isKamaeFrame = 0;
	}

	// アニメーションと動きを設定
	switch (_state)
	{
	case StateID::Idle: // 未設定
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
		SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, kLegWalkBox, kArmBox);
		SetActionInfo(m_speed * kDashSpeed, 0, 10, true, true);
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
		SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, kLegWalkBox, kArmBox);
		SetActionInfo(-m_speed * kDashSpeed, 0, 10, true, true);
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
	case StateID::Shoryu:
		m_isShoryu = true;
		ChangeAnim(AnimID::Shoryu);
		ChangeColType(ColType::Hurt);
		SetActionInfo(0, 0, 0, true, true);
		SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, {}, kArmBox);
		m_colmap[static_cast<int>(ColType::PushBack)][static_cast<int>(ColDetail::Body)].active = false;
		m_mainDamage = kShoryuDamage;
		m_isAttackState = true;
		break;

	case StateID::Kamae:
		m_isKamae = true;
		ChangeAnim(AnimID::Kamae);
		ChangeColType(ColType::Hurt);
		SetActionInfo(0, 0, 0, true, false);
		SetBoxBodyHead(ColType::Hurt, kBodyKickBox, kHeadKickBox, kLegKickBox, kArmBox);
		break;

	case StateID::Totsu:
		ChangeAnim(AnimID::Totsu);
		ChangeColType(ColType::Hurt);
		// 構えの時間によってスピードを変更する
		if (m_isKamaeFrame < 30)
		{
			m_totsuSpeed = kTotsuSpeedLv1;
			m_mainDamage = kTotsuDamageLv1;
			m_attackScale = AttackType::S;
		}
		else if(m_isKamaeFrame < 60)
		{
			m_totsuSpeed = kTotsuSpeedLv2;
			m_mainDamage = kTotsuDamageLv2;
			m_attackScale = AttackType::M;
		}
		else
		{
			m_totsuSpeed = kTotsuSpeedLv3;
			m_mainDamage = kTotsuDamageLv3;
			m_attackScale = AttackType::Down;
		}

		SetActionInfo(m_speed * m_totsuSpeed, 0, 10, true, false);
		SetBoxBodyHead(ColType::Hurt, kBodyKickBox, kHeadKickBox, kLegKickBox, kArmBox);
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

	case StateID::Damaged:
		m_isKamae = false;

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
			SetActionInfo(m_speed * kKnockBackSpeed, 0, 30, true, false);
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
		m_isKamae = false;

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
			SetActionInfo(m_speed * kDownKnockBackSpeed, -20, 30, true, false);
			SetBoxBodyHead(ColType::Invisible, kBodyBox, kHeadBox, kLegBox, kArmBox);
			ChangeColType(ColType::Invisible);
		}
		else
		{
			ChangeAnim(AnimID::AirDamaged);
			ChangeColType(ColType::Invisible);
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
				m_isKamae = false;

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

	if (m_currentAnimID == AnimID::Kick && m_dir == -1)
	{
		m_adjShartelSlashPosX = kAdjShartelSlashPos;
	}
	else
	{
		m_adjShartelSlashPosX = 0;
	}
}

void Shartel::SetActionDetail()
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
	else if (m_currentAnimID == AnimID::Totsu)
	{
		if (m_pCurrentAnim->RequestAnimIdxAndFrameRange(4, 4, 4, 5))
		{
			m_virtualMove.x = 0;
		}
	}
	else if (m_currentAnimID == AnimID::Shoryu)
	{
		if (m_pCurrentAnim->RequestAnimIdxAndFrameRange(0, 0, 9, 10))
		{
			SetActionInfo(m_speed * kJumpSpeedX, kJumpSpeed, 0, false, false);
			m_countShoryu++;
		}
	}
	else if (m_currentAnimID == AnimID::JumpForward || m_currentAnimID == AnimID::JumpBack)
	{
		m_colmap[static_cast<int>(ColType::PushBack)][static_cast<int>(ColDetail::Body)].active = true;
	}
	else if (m_currentAnimID == AnimID::DownDamaged)
	{
		if (m_pCurrentAnim->RequestAnimIdxAndFrameRange(7, 7, 4, 5))
		{
			m_virtualMove.x = 0;
		}
	}
}

void Shartel::SetAttackCol()
{
	switch (m_currentAnimID)
	{
	case AnimID::Punch:
		// 指定したフレームの間コリジョンを生成する
		SetColBox(ColType::HitTop, ColDetail::Arm, kSPunchBox);
		GeneCol(ColType::HitTop, ColDetail::Arm, 2, 2, 0, 5);
		break;

	case AnimID::Kick:
		// 足の上段攻撃のコリジョンを設定する（竜巻に上書きされるから）
		SetColBox(ColType::HitTop, ColDetail::Leg, kKickBox);
		GeneCol(ColType::HitTop, ColDetail::Leg, 1, 1, 0, 5);
		break;

	case AnimID::KickBottom:
		GeneCol(ColType::HitBottom, ColDetail::Leg, 2, 2, 0, 5);
		break;

	case AnimID::JumpPunch:
		GeneCol(ColType::HitMiddle, ColDetail::Arm, 1, 2, 0, 5);
		break;

	case AnimID::JumpKick:
		GeneCol(ColType::HitMiddle, ColDetail::Leg, 0, 1, 5, 3);
		break;

	case AnimID::Totsu:
		// 指定したフレームの間コリジョンを生成する
		SetColBox(ColType::HitTop, ColDetail::Arm, kTotsuBox);
		GeneCol(ColType::HitTop, ColDetail::Arm, 2, 3, 0, 5);
		break;

	// 昇竜
	case AnimID::Shoryu:
		SetColBox(ColType::HitTop, ColDetail::Arm, kKickBox);
		GeneCol(ColType::HitTop, ColDetail::Arm, 0, 1, 3, 8);
		m_attackScale = AttackType::S;
		break;
	case AnimID::ShoryuUp:
		SetColBox(ColType::HitTop, ColDetail::Arm, kShoryuBox);
		GeneCol(ColType::HitTop, ColDetail::Arm, 0, 1, 0, 8);
		m_attackScale = AttackType::Down;
		break;
	case AnimID::ShoryuTop:
		SetColBox(ColType::HitTop, ColDetail::Arm, kShoryuBox);
		GeneCol(ColType::HitTop, ColDetail::Arm, 0, 0, 0, 4);
		break;
	case AnimID::ShoryuFall:
		// 押し戻し判定をアクティブにする
		m_colmap[static_cast<int>(ColType::PushBack)][static_cast<int>(ColDetail::Body)].active = true;
		break;
	// ここまで
	}
}

StateID Shartel::InputAction(const Pad& _pad)
{
	if (m_isKamae && (_pad.Release(ButtonType::A) || _pad.Release(ButtonType::Y)) && m_isLanding)
	{
		m_isKamae = false;
		if (_pad.Press(ButtonType::Down))
		{
			m_isAction = false;
			return StateID::Crouch;
		}
		else
		{
			return StateID::Totsu;
		}
	}

	// インプット
	if (!m_isAction && m_coolDownTimer <= 0 && m_isLanding)
	{
		if (_pad.Trigger(ButtonType::A) || _pad.Trigger(ButtonType::Y))
		{
			// コマンドによる攻撃を優先して取得する
			/*if (m_commandManager.CheckCommand(m_dir) == "Tatsumaki")
			{
				// 構えに遷移
				return StateID::Kamae;
			}*/

			if (m_commandManager.IsCheckCommand(CommandID::Tatsumaki, m_dir))
			{
				return StateID::Kamae;
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
			/*if (m_commandManager.CheckCommand(m_dir) == "Shoryu")
			{
				// 波動拳をするとき
				return StateID::Shoryu;
			}*/

			if (m_commandManager.IsCheckCommand(CommandID::Shoryu, m_dir))
			{
				return StateID::Shoryu;
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
			if (m_commandManager.IsCheckCommand(CommandID::DashBack, m_dir))
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
			if (m_commandManager.IsCheckCommand(CommandID::DashForward, m_dir))
			{
				return StateID::DashForward;
			}
			else
			{
				return StateID::WalkForward;
			}
		}
		// どこにも当てはまらない場合はアイドル状態
		else
		{
			return StateID::Idle;
		}
	}
	// 空中にいるときに行う行動
	else if (!m_isAction && m_coolDownTimer <= 0 && !m_isLanding)
	{
		if (_pad.Trigger(ButtonType::A) || _pad.Trigger(ButtonType::Y))
		{
			return StateID::JumpKick;
		}
		else if (_pad.Trigger(ButtonType::X) || _pad.Trigger(ButtonType::B))
		{
			return StateID::JumpPunch;
		}
	}
	return StateID::None;
}