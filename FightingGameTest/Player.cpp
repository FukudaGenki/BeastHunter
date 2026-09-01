#include "DxLib.h"
#include "Player.h"
#include "Pad.h"
#include "SoundManager.h"

// プレイヤーあとやること
// 攻撃を当てた時下がるヒットバック

namespace
{
	constexpr int kGraficSize = 480;

	// 速度
	constexpr float kSpeed = 3;
	constexpr float kPushBackSpeed = 1.5f;
	constexpr float kTatsuSpeed = 1.4f;
	constexpr float kDashSpeed = 3.0f;
	constexpr float kRollSpeed = 2.0f;
	constexpr float kJumpRollSpeedX = 3.0f;
	constexpr float kJumpRollSpeedY = 10.0f;
	constexpr float kKnockBackSpeed = -1.0f;
	constexpr float kHitBackSpeed = -2.0f;
	// ダメージ
	constexpr float kLDamageSpeed = -0.7f;
	constexpr float kMDamageSpeed = -1.5f;
	constexpr float kSDamageSpeed = -0.2f;

	// クールダウン
	// アニメーションが終了してからの時間
	// 移動関連
	constexpr int kDashCD = 2;
	// 攻撃関連
	constexpr int kPunchCD = 5;
	constexpr int kKickCD = 15;
	constexpr int kRollCD = 20;
	constexpr int kHadouCD = 10;
	constexpr int kTatsuCD = 7;

	// ジャンプ
	constexpr float kJumpSpeed = -35.0f;
	constexpr float kJumpSpeedX = 1.7f;
	constexpr float kGravityAccel = 1.2f;
	constexpr float kFallGravityScale = 1.5f;
	constexpr float kGroundPos = Game::kScreenHeight * 0.375f;

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
	constexpr Box kBodyKickBox = kBodyCrouchBox;
	constexpr Box kBodyRollBox = { kGraficSize * 0.325f, kGraficSize * 0.325f, kGraficSize * 0.35f, kGraficSize * 0.35f };
	constexpr Box kBodyKickBottomBox = { kGraficSize * 0.35f, kGraficSize * 0.8f, kGraficSize * 0.25f, kGraficSize * 0.2f };
	constexpr Box kBodyTatsuBox = { kGraficSize * 0.35f, kGraficSize * 0.35f, kGraficSize * 0.4f, kGraficSize * 0.65f };
	// 頭
	constexpr Box kHeadBox = { kGraficSize * 0.4125f, kGraficSize * 0.2f, kGraficSize * 0.175f, kGraficSize * 0.225f };
	constexpr Box kHeadDashForwardBox = { kGraficSize * 0.6f, kGraficSize * 0.45f, kGraficSize * 0.2f, kGraficSize * 0.2f };
	constexpr Box kHeadCrouchBox = { kGraficSize * 0.45f, kGraficSize * 0.45f, kGraficSize * 0.25f, kGraficSize * 0.3f };
	constexpr Box kHeadPunchBox = { kGraficSize * 0.6f, kGraficSize * 0.25f, kGraficSize * 0.2f, kGraficSize * 0.2f };
	constexpr Box kHeadKickBox = kHeadCrouchBox;
	constexpr Box kHeadKickBottomBox = { kGraficSize * 0.2f, kGraficSize * 0.6f, kGraficSize * 0.2f, kGraficSize * 0.4f };
	constexpr Box kHeadTatsuBox = { kGraficSize * 0.5f, kGraficSize * 0.5f, kGraficSize * 0.2f, kGraficSize * 0.2f };
	// 足
	constexpr Box kLegBox = { kGraficSize * 0.325f, kGraficSize * 0.6f, kGraficSize * 0.35f, kGraficSize * 0.4f };
	constexpr Box kLegDashForwardBox = { kGraficSize * 0.1f, kGraficSize * 0.45f, kGraficSize * 0.35f, kGraficSize * 0.3f };
	constexpr Box kLegCrouchBox = { kGraficSize * 0.375f, kGraficSize * 0.7f, kGraficSize * 0.25f, kGraficSize * 0.3f }; // ここゼロ
	constexpr Box kLegWalkBox = { kGraficSize * 0.375f, kGraficSize * 0.6f, kGraficSize * 0.25f, kGraficSize * 0.4f };
	constexpr Box kLegKickBox = kLegCrouchBox;
	constexpr Box kLegKickBottomBox = { kGraficSize * 0.53f, kGraficSize * 0.8f, kGraficSize * 0.5f, kGraficSize * 0.2f };
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
	constexpr Box kSPunchBox = { kGraficSize * 0.7f, kGraficSize * 0.2f, kGraficSize * 0.2f, kGraficSize * 0.2f };
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

Player::Player()
{

}

Player::~Player()
{
	// アニメーションのデリートはAnimationクラスのデストラクタに記述
}

void Player::Init(float _posX, int _pIdx)
{
	// 仮
	m_hitStopFrame = 10;

	// 位置の初期化
	m_pos.x = _posX;
	m_pos.y = kGroundPos;
	m_speed = kSpeed;
	m_graficSize = kGraficSize;

	SetAnimData();
	SetColData();

	// 変数の初期化
	m_pCurrentAnim = &m_animations[AnimID::Idle];
	m_isAction = false;
	m_isLanding = true;
	m_isJumpAttack = false;
	m_hp = kMaxHp;

	// インスタンスの初期化
	m_commandManager.Init(_pIdx);
}

void Player::SecondInit(float _posX)
{
	// 仮
	//m_hitStopFrame = 10;

	// 位置の初期化
	m_pos.x = _posX;
	m_speed = kSpeed;
	m_pos.y = kGroundPos;
	m_virtualMove = { 0, 0 };
	m_move = { 0, 0 };

	//SetAnimData();
	//SetColData();

	//変数の初期化
	m_pCurrentAnim = &m_animations[AnimID::Idle];
	m_isAction = false;
	m_isLanding = true;
	m_isJumpAttack = false;
	m_hp = kMaxHp;
	m_isLastDamaged = false;
	m_isAttackState = false;

	// インスタンスの初期化
	//m_commandManager.Init();
}

void Player::Update(const Pad& _pad, bool isFinishGame)
{
	m_commandManager.Update(_pad);
	ActionManager(_pad);
	UpdateCollision();
}

void Player::UpdateMove(bool _isStart)
{
	// 攻撃を撃っているなら空振りSEを流す
	if (m_isAttackState)
	{
		m_isAttackState = false;
		PlayWhiffSE();
	}

	// 技が当たった時、後ろに下がる
	// ヒットバック
	if (m_isHitBack && m_hitBackAction)
	{
		m_virtualMove.x = kHitBackSpeed * m_dir;
	}
	m_isStart = _isStart;

	// 当たり判定や描画の反転を判定する変数を変更
	if (m_dir == 1 && !m_isAction && m_isLanding)
	{
		m_isTurn = false;
	}
	else if (m_dir == -1 && !m_isAction && m_isLanding)
	{
		m_isTurn = true;
	}
	// 押し戻しの処理
	switch (m_pushBack)
	{
	case PushBackType::Balance:
		m_virtualMove.x = 0;
		break;
	case PushBackType::Push:
		m_virtualMove.x = kPushBackSpeed * m_dir; // 仮
		break;
	case PushBackType::Pushed:
		if (m_isLanding)m_virtualMove.x = -kPushBackSpeed * m_dir;
		else if(!m_isLanding) m_virtualMove.x = -kPushBackSpeed * m_dir * 3;
		break;
	}
	m_pos.x += m_scrollX;

	// プレイヤーの動きを更新
	// ヒットストップ中は動きを止める
	// ヒットストップ中でないとき
	if (m_hitStopFrameTimer < 0 || !m_isStart)
	{
		m_move = m_virtualMove;
	}
	else
	{
		m_move = {};
	}

	Box pbCol = m_colmap[static_cast<int>(ColType::PushBack)][static_cast<int>(ColDetail::Body)].box;
	if (GetPBLeftEdge() <= 0 && m_move.x <= 0)
	{
		m_pos.x = -pbCol.x;
		m_move.x = 0;
	}
	else if (GetPBRightEdge() >= Game::kScreenWidth && m_move.x >= 0)
	{
		m_pos.x = Game::kScreenWidth - (m_graficSize - pbCol.x);
		m_move.x = 0;
	}

	if (!m_isStart)
	{
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
	}
}

void Player::Draw()
{
	m_pCurrentAnim->Draw(m_pos.x - m_adjShartelSlashPosX, m_pos.y - m_graficDrawAdjY, m_isTurn);
	m_commandManager.Draw();
	//DrawFormatString(500, 300, 0x000000, "count:%d", m_countShoryu);

#ifdef _DEBUG
	//hpを表示
	DrawFormatString(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 0xffffff, "hitBack :%d", m_isHitBack);

	// 当たり判定の描画
	for (int i = 0; i < static_cast<int>(ColType::Max); i++)
	{
		unsigned int color = 0;
		if (i == static_cast<int>(ColType::Hurt))
		{
			// 青
			color = 0x0000ff;
		}
		else if (i == static_cast<int>(ColType::HitTop))
		{
			// 赤
			color = 0xff0000;
		}
		else if (i == static_cast<int>(ColType::HitBottom))
		{
			// 
			color = 0x00ffff;
		}
		else if (i == static_cast<int>(ColType::GuardTop) || i == static_cast<int>(ColType::PushBack))
		{
			// 緑
			color = 0x00ff00;
		}
		else if (i == static_cast<int>(ColType::GuardBottom))
		{
			// 紫
			color = 0xff00ff;
		}
		else if (i == static_cast<int>(ColType::Invisible))
		{
			// 白
			color = 0xffffff;
		}
		else if (i == static_cast<int>(ColType::HitMiddle))
		{
			color = 0xffff00;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		for (int j = 0; j < static_cast<int>(ColDetail::Max); j++)
		{
			m_colmap[i][j].col.Draw(color, true);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		for (int j = 0; j < static_cast<int>(ColDetail::Max); j++)
		{
			m_colmap[i][j].col.Draw(color, false);
		}
	}
#endif
}

void Player::SetScrollX(float _scrollX)
{
	m_scrollX = _scrollX;
}

void Player::SetRightLeft(int _rightLeft)
{
	if (m_isLanding)
	{
		m_dir = _rightLeft;
	}
}

void Player::UpdateCollision()
{
	// 当たり判定がアクティブなら生成、非アクティブなら消す
	// すべての当たり判定の種類を調べる
	for (int i = 0; i < static_cast<int>(ColType::Max); i++)
	{
		for (int j = 0; j < static_cast<int>(ColDetail::Max); j++)
		{
			if (m_colmap[i][j].active)
			{
				// 生成
				SetCol(m_colmap[i][j].box, m_colmap[i][j].col);
			}
			else if (!m_colmap[i][j].active)
			{
				// オフセット
				m_colmap[i][j].col.OffSet();
			}
		}
	}

	// 攻撃など一時的に生成する当たり判定の処理
	SetAttackCol();
}

void Player::ChangeColType(ColType _col)
{
	// 当たり判定の状態が変わらないなら処理しない
	if (m_currentCol == _col) return;
	// currentCol変更前の処理
	int lastColIdx = static_cast<int>(m_currentCol);
	for (int i = 0; i < static_cast<int>(ColDetail::Max); i++)
	{
		// あらかじめ設定していない当たり判定ははじく
		if (m_colmap[static_cast<int>(ColType::Hurt)][i].box.x != 0)
		{
			// 指定した当たり判定以外をオフにする
			m_colmap[lastColIdx][i].active = false;
		}
	}

	// currentCol変更後の処理
	m_currentCol = _col;
	int colIdx = static_cast<int>(m_currentCol);

	for (int i = 0; i < static_cast<int>(ColDetail::Max); i++)
	{
		// あらかじめ設定していない当たり判定ははじく
		if (m_colmap[static_cast<int>(ColType::Hurt)][i].box.x != 0)
		{
			// 指定した当たり判定をオンにする
			m_colmap[colIdx][i].active = true;
		}
	}
}

void Player::GeneCol(ColType _col, ColDetail _colDetail, int _animIdxMin, int _animIdxMax, int _frameMin, int _frameMax)
{
	if (m_pCurrentAnim->RequestAnimIdxAndFrame(_animIdxMin, _frameMin))
	{
		m_isHitOnce = false;
	}
	// 指定されたフレームで発動
	if (m_pCurrentAnim->RequestAnimIdxAndFrameRange(_animIdxMin, _animIdxMax, _frameMin, _frameMax))
	{
		m_colmap[static_cast<int>(_col)][static_cast<int>(_colDetail)].active = true;
	}
	else
	{
		m_colmap[static_cast<int>(_col)][static_cast<int>(_colDetail)].active = false;
	}

	/*bool isAttackCol = _col == ColType::HitTop || _col == ColType::HitMiddle || _col == ColType::HitBottom;
	if (isAttackCol)
	{
		PlayWhiffSE()
	}*/
}

void Player::ActionManager(const Pad& _pad)
{
	m_speed = kSpeed * m_dir;
	m_coolDownTimer--;
	m_hitStopFrameTimer--;
	m_isJumpingFrame = false;

	// 現在のアクション状態を取得
	StateID currentState = GetCurrentAction(_pad);

	// アニメーションや動きをステートに合わせる
	if(!m_isStart)SetCurrentAction(currentState);

	// 関係のない値で初期化する
	m_damagedType = ColType::Max;
	
	// ヒットストップを与える
	if (m_isHit)
	{
		m_isHit = false;
		// 小ダメージのみアニメーションを流す前にヒットストップを与える
		if (m_currentAnimID != AnimID::sDamaged)
		{
			m_pCurrentAnim->StopAnim(m_hitStopFrame);
		}
		m_hitStopFrameTimer = m_hitStopFrame;
	}

	// アニメーションの更新処理
	m_pCurrentAnim->Update();

	// 細かいところ（フレーム指定で行う処理）
	SetActionDetail();

	Gravity();
}

void Player::Gravity()
{
	float gravityScale = kGravityAccel;
	if (!m_isLastDamaged)
	{
		// 接地判定
		if (m_pos.y >= kGroundPos && !m_isJumpingFrame)
		{
			if (!m_isLanding)
			{
				m_isLanding = true;
				m_isJumpAttack = false;
				m_offGravity = false;
				if (!m_isDownDamaged)
				{
					ChangeAnim(AnimID::JumpLanding);
					SetActionInfo(0, 0, 0, true, false);
					SetBoxBodyHead(ColType::Hurt, kBodyBox, kHeadBox, kLegBox, kArmBox);
				}
				else
				{
					SetActionInfo(0, 0, 15, true, false);
					m_isDownDamaged = false;
					SetBoxBodyHead(ColType::Invisible, kBodyBox, kHeadBox, kLegBox, kArmBox);
				}
				
				DeactiveHitCol();
			}
			m_virtualMove.y = 0;
			m_pos.y = kGroundPos;
		}
		// 空中にいるとき
		else
		{
			m_isLanding = false;
			// 昇竜の処理
			if (!m_damaged && m_isShoryu)
			{
				m_isJumpAttack = false;
				if (m_virtualMove.y < kJumpingToTopSpeed)
				{
					ChangeAnim(AnimID::ShoryuUp);
				}
				else if (m_virtualMove.y >= kJumpingToTopSpeed && m_virtualMove.y < kTopToFallingSpeed)
				{
					ChangeAnim(AnimID::ShoryuTop);
				}
				else if (m_virtualMove.y >= kTopToFallingSpeed)
				{
					ChangeAnim(AnimID::ShoryuFall);
					m_isShoryu = false;
				}
			}

			// 空中攻撃を行っていないなら処理を行う
			if (!m_isJumpAttack && !m_damaged && !m_isShoryu)
			{
				// ニュートラルジャンプ
				if (m_virtualMove.x == 0)
				{
					if (m_virtualMove.y < kJumpingToTopSpeed)
					{
						ChangeAnim(AnimID::Jumping);
					}
					else if (m_virtualMove.y >= kJumpingToTopSpeed && m_virtualMove.y < kTopToFallingSpeed)
					{
						ChangeAnim(AnimID::JumpTop);
					}
					else if (m_virtualMove.y >= kTopToFallingSpeed)
					{
						ChangeAnim(AnimID::JumpFalling);
					}
				}
				// バックジャンプ
				else if (m_virtualMove.x * m_dir < 0)
				{
					ChangeAnim(AnimID::JumpBack);
				}
				// 前ジャンプ
				else if (m_virtualMove.x * m_dir > 0)
				{
					ChangeAnim(AnimID::JumpForward);
				}
			}
	}

		if (m_virtualMove.y >= kTopToFallingSpeed)
		{
			// 落ちるときは加速度を増やす
			gravityScale *= kFallGravityScale;
		}
		if (!m_offGravity)m_virtualMove.y += gravityScale;
	}
}

void Player::ChangeAnim(AnimID _nextID)
{
	if (m_changeSameAnim)
	{
		m_pCurrentAnim->Reset();
		m_changeSameAnim = false;
	}
	// アニメーションが変わらないなら何も行わない
	if (m_currentAnimID == _nextID) return;
	// IDを更新
	m_currentAnimID = _nextID;
	// 再生するアニメーションのポインタを切り替える
	m_pCurrentAnim = &m_animations[_nextID];
	// アニメーションの初期化
	m_pCurrentAnim->Reset();
}

void Player::SetActionInfo(float _moveX, float _moveY, int _coolDown, bool _action, bool _cdAnim)
{
	// X軸の動き
	m_virtualMove.x = _moveX;
	m_virtualMove.y = _moveY;
	if (_moveY < 0)
	{
		m_isJumpingFrame = true;
		m_isLanding = false;
	}
	// キャンセルできる行動ならなら以下の処理は行わない
	if (_coolDown <= 0 && !_action) return;
	// クールダウン
	m_coolDownTimer = m_pCurrentAnim->GetAllFrame() + _coolDown;
	// キャンセルのできないアクションかどうか（基本的には攻撃）
	m_isAction = _action;
	m_cdAnim = _cdAnim;
}

// インプット
// 弱P INPUT_3
// 中P INPUT_4
// 弱K INPUT_1
// 中K INPUT_2

StateID Player::GetCurrentAction(const Pad& _pad)
{
	if (m_damagedType == ColType::GuardTop)
	{
		// 昇竜をダメージを受けた時にキャンセルさせる
		m_isShoryu = false;
		// 押し戻し判定を復活させる（押し戻し判定をなくす技があるため）
		m_colmap[static_cast<int>(ColType::PushBack)][static_cast<int>(ColDetail::Body)].active = true;
		return StateID::GuardTop;
	}
	else if (m_damagedType == ColType::Hurt)
	{
		m_isShoryu = false;
		if (m_isLanding)
		{
			return StateID::Damaged;
		}
		else if (!m_isLanding)
		{
			return StateID::AirDamaged;
		}
	}
	
	return InputAction(_pad);

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

void Player::SetPos(Vec2 _pos)
{
	m_pos = _pos;
}

Vec2 Player::GetCenterPos() const
{
	Vec2 centerPos = {};
	centerPos.x = m_pos.x + kGraficSize * 0.5f;
	centerPos.y = m_pos.y + kGraficSize * 0.5f;
	return centerPos;
}

void Player::SetCol(Box _box, Rect& _col)
{
	float left = m_pos.x + _box.x;
	if (m_isTurn)
	{
		left = m_pos.x + (m_graficSize - (_box.width + _box.x));
	}

	_col.SetLT(
		left, m_pos.y + _box.y, // ポジション
		_box.width, _box.height // 大きさ
	);
}

std::vector<Rect> Player::GetCol(ColType _type) const
{
	// 結果をいれるための変数
	std::vector<Rect> result;
	// typeの実数
	int typeIdx = static_cast<int>(_type);

	// ディテールを確認していく
	for (int i = 0; i < static_cast<int>(ColDetail::Max); ++i)
	{
		if (m_colmap[typeIdx][i].active)
		{
			result.push_back(m_colmap[typeIdx][i].col);
		}
	}

	// 当たり判定の配列を返す
	return result;
}

void Player::SetIsHit(bool _hit)
{
	// 攻撃フラグを立てる
	m_isHit = _hit;
	m_isHitOnce = _hit;
	PlayHitSE();
}

void Player::IsDamaged(int _damage, AttackType _type)
{
	// アニメーションの変更
	m_damagedType = ColType::Hurt;
	// ダメージの受け渡し
	m_hp -= _damage;
	if (m_hp <= 0) m_isLastDamaged = true;
	m_damagedScale = _type;
	// ヒットストップ
	m_isHit = true;
	PlayGuardSE();
}

void Player::IsGuard(int _damage)
{
	// アニメーションの変更
	m_damagedType = ColType::GuardTop;
	// ヒットストップ
	m_isHit = true;
}

void Player::SetColBox(ColType _col, ColDetail _colDetail, Box _box)
{
	m_colmap[static_cast<int>(_col)][static_cast<int>(_colDetail)].box = _box;
}

void Player::SetFrameColBox(ColType _col, ColDetail _colDetail, int _animNum,int _animFrame, Box _box)
{
	if (m_pCurrentAnim->RequestAnimIdxAndFrame(_animNum, _animFrame))
	{
		m_isHitOnce = false;
	}
	// 指定されたフレームで発動
	if (m_pCurrentAnim->RequestAnimIdx(_animFrame))
	{
		m_colmap[static_cast<int>(_col)][static_cast<int>(_colDetail)].box = _box;
	}
}

void Player::SetBoxBodyHead(ColType _col, Box _body, Box _head, Box _leg, Box _arm)
{
	m_colmap[static_cast<int>(_col)][static_cast<int>(ColDetail::Body)].box = _body;
	m_colmap[static_cast<int>(_col)][static_cast<int>(ColDetail::Head)].box = _head;
	m_colmap[static_cast<int>(_col)][static_cast<int>(ColDetail::Leg)].box = _leg;
	m_colmap[static_cast<int>(_col)][static_cast<int>(ColDetail::Arm)].box = _arm;
}

void Player::ResetAttackCol()
{
	for (int i = 0; i < static_cast<int>(ColDetail::Max); i++)
	{
		m_colmap[static_cast<int>(ColType::HitMiddle)][i].active = false;
		m_colmap[static_cast<int>(ColType::HitTop)][i].active = false;
		m_colmap[static_cast<int>(ColType::HitBottom)][i].active = false;
		m_colmap[static_cast<int>(ColType::Hit)][i].active = false;
	}
}

void Player::PlayHitSE()
{
	SoundManager::Instance().PlaySE(SoundType::Hit);
}

void Player::PlayGuardSE()
{
	SoundManager::Instance().PlaySE(SoundType::Guard);
}

void Player::PlayWhiffSE()
{
	SoundManager::Instance().PlaySE(SoundType::Whiff);
}

void Player::DeactiveHitCol()
{
	for (int i = 0; i < static_cast<int>(ColDetail::Max); i++)
	{
		m_colmap[static_cast<int>(ColType::HitTop)][i].active = false;
		m_colmap[static_cast<int>(ColType::HitMiddle)][i].active = false;
		m_colmap[static_cast<int>(ColType::HitBottom)][i].active = false;
	}
}

void Player::SetIsPushBack(PushBackType _pushBack)
{
	m_pushBack = _pushBack;
}

int Player::GetPlayerMaxHP() const
{
	return kMaxHp;
}

float Player::GetPBRightEdge() const
{
	// 押し戻し判定のBoxを取得
	Box pb = m_colmap[static_cast<int>(ColType::PushBack)][static_cast<int>(ColDetail::Body)].box;
	// 右向き
	if (m_dir == 1)
	{
		return m_pos.x + pb.x + pb.width;
	}
	// 左向き
	else if (m_dir == -1)
	{
		return m_pos.x + m_graficSize - pb.x;
	}
	else
	{
		return 0;
	}
}

float Player::GetPBLeftEdge() const
{
	// 押し戻し判定のBoxを取得
	Box pb = m_colmap[static_cast<int>(ColType::PushBack)][static_cast<int>(ColDetail::Body)].box;

	if (m_dir == 1)
	{
		return m_pos.x + pb.x;
	}
	else if (m_dir == -1)
	{
		return m_pos.x + (m_graficSize - (pb.x + pb.width));
	}
	else
	{
		return 0;
	}
}

float Player::GetPBTopEdge() const
{
	Box pb = m_colmap[static_cast<int>(ColType::PushBack)][static_cast<int>(ColDetail::Body)].box;

	return pb.y + m_pos.y;
}

bool Player::GetIsRightEdge() const
{
	float rightPos = GetPBRightEdge();
	if (rightPos >= Game::kScreenWidth)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::GetIsLeftEdge() const
{
	float leftPos = GetPBLeftEdge();
	if (leftPos <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
