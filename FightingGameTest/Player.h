#pragma once
#include "Animation.h"
#include "GameDefine.h"
#include "InputBuffer.h"
#include "CommandManager.h"
#include <map>
#include <string>
#include "Vec2.h"
#include "Rect.h"
#include "ButtonType.h"
#include <vector>
// プレイヤーのアニメーション
enum class AnimID
{
	Idle,
	// ジャンプの状態
	JumpFront,
	Jumping,
	JumpForward,
	JumpBack,
	JumpTop,
	JumpFalling,
	JumpLanding,
	// 移動の状態
	WalkForward,
	DashForward,
	WalkBack,
	// しゃがみの状態
	Crouch,
	// ガードの状態
	GuardTop,
	GuardBottom,
	// 攻撃の状態
	Punch,
	Kick,
	KickBottom,
	// コマンドの攻撃
	// Nova
	Hadou,
	Tatsumaki,
	Roll,
	Kamae,
	Totsu,
	// Shartel
	Shoryu,
	ShoryuUp,
	ShoryuTop,
	ShoryuFall,
	// 空中攻撃の状態
	JumpPunch,
	JumpKick,
	// ダメージ
	sDamaged,
	Damaged,
	LLDamaged,
	AirDamaged,
	DownDamaged,
	LastDamaged
};

// プレイヤーの状態
enum class StateID
{
	Idle,
	// ジャンプ
	JumpNeutral,
	JumpForward,
	JumpBack,
	// 移動
	WalkForward,
	DashForward,
	WalkBack,
	DashBack,
	// しゃがみ
	Crouch,
	CrouchBack,
	// ガード
	GuardTop,
	GuardBottom,
	// 攻撃
	Punch,
	Kick,
	KickBottom,
	// コマンド攻撃
	Hadou,
	Tatsumaki,
	Roll,
	Shoryu,
	Kamae,
	Totsu,
	// 空中攻撃
	JumpPunch,
	JumpKick,
	JumpRoll,
	// ダメージ状態
	Damaged,
	AirDamaged,
	// エラーの場合
	None
};

// アニメーションの構造体
struct AnimData
{
	const char* filePath; // 画像のパス
	int animFrame; // 1コマの表示フレーム数
	int animNum; // アニメーションのコマ数
	bool isLoop; // ループするかどうか
};

struct Box
{
	// 位置X
	float x;
	// 位置Y
	float y;
	// 幅
	float width;
	// 高さ
	float height;
};

enum class ColID
{
	Body,
	Head,
	Attack,
	// 当たり判定の種類の総数を表す
	ColMax
};

enum class ColType
{
	// やられ
	Hurt,
	// 押し戻し
	PushBack,
	// 攻撃
	Hit,
	HitMiddle,
	HitTop,
	HitBottom,
	// ガード
	GuardTop, // 上段
	GuardBottom, // 下段
	// 無敵
	Invisible,
	Max
};

enum class ColDetail
{
	// Hurt
	Body,
	Head,
	Leg,
	Arm,

	// 最大数
	Max
};

enum class PushBackType
{
	Balance,
	Push,
	Pushed,
	None
};

enum class AttackType
{
	S, // 小攻撃
	M, // 中攻撃
	L, // 大攻撃
	Down,
	None
};

struct ColData
{
	Rect col;
	Box box;
	bool active;
};

class Player 
{
protected:
	// グラフィックの大きさの関係で位置を調整するための変数
	float m_graficDrawAdjY = 0.0f;
	// すべてのアニメーションをIDで紐づけて管理するマップ
	std::map<AnimID, Animation> m_animations;
	// 同じアニメーションを連続で遷移させるときに使う変数
	bool m_changeSameAnim = false;
	// 当たり判定
	ColData m_colmap[static_cast<int>(ColType::Max)][static_cast<int>(ColDetail::Max)] = {};

protected:
	// 攻撃の状態に変わったフレームでのみ立てるフラグ
	bool m_isAttackState = false;

	int m_countShoryu = 0;

	int m_animIdx = 0;
	int m_frameTimer = 0;
	Vec2 m_pos = { 0, Game::kScreenHeight * 0.5f };
	float m_scrollX = 0;
	float m_speed = 0;
	Vec2 m_move = { 0, 0 };
	Vec2 m_virtualMove = {};
	int m_coolDownTimer = 0;

	// 画像サイズ
	int m_graficSize = 0;

	// スタート時動かないようにするための変数
	bool m_isStart = false;

	// ジャンプに使う変数
	bool m_isLanding = 0;
	bool m_isJumpingFrame = 0;
	bool m_isJumpAttack = 0;
	bool m_offGravity = 0;

	// コマンドで使う変数
	bool m_isHadou = false;
	bool m_isShoryu = false;

	// 攻撃に使う変数
	bool m_isPunch = false;
	// 攻撃の強さ（小・中・大）
	AttackType m_attackScale = AttackType::None;
	// 攻撃が敵に当たった時
	bool m_isHit = false;
	// 一度だけ当てるための変数
	bool m_isHitOnce = false;
	// 攻撃を当てた時のヒットバック
	// 1か0
	int m_hitBackSpeed = 0;
	// 敵プレイヤーが画面端なら後ろにやる
	bool m_isHitBack = false;
	// ヒットバックを行う攻撃かどうか
	bool m_hitBackAction = false;
	// ダウンをするときの変数
	bool m_isDownDamaged = false;
	// しゃがみで使う変数
	bool m_isCrouch = false;

	// ヒットストップのフレーム数を設定
	int m_hitStopFrame = 0;
	int m_hitStopFrameTimer = 0;

	// 押し戻しで使う変数
	PushBackType m_pushBack = PushBackType::None;

	// 相手からの攻撃を受けた時、ダメージを受けるorガード
	ColType m_damageType = ColType::Max;

	// 入力関連
	CommandManager m_commandManager;

	bool m_isAction = false; // キャンセルできないアクションかどうか
    // CD時にfalse:アニメーションの最後のコマでとめる、true:IdleAnimへ遷移
	bool m_cdAnim = false;

	// ガードorダメージ状態を表す
	bool m_damaged = false;
	// ダメージを受けた時の敵のアタック強度
	AttackType m_damagedScale = AttackType::None;

	// ダメージを受けた時のプレイヤーの状態を表す（上段ガード、ダメージなど）
	ColType m_damagedType = ColType::Max;

	Animation* m_pCurrentAnim = nullptr; // 現在再生中のアニメーションのポインタ
	AnimID m_currentAnimID = AnimID::Idle; // 現在のアニメーションID
	AnimID m_lastAnimID = AnimID::Idle; // 前フレームのアニメーションID

	ColType m_currentCol = ColType::Hurt; // 体の当たり判定を管理する

	// 向きの管理に使う変数
	// -1：左向き、1：右向き
	int m_dir = 0;
	bool m_isTurn = false;

	// HPに関する変数
	int m_hp = 0;
	int m_mainDamage = 0;
	// プレイヤーが死ぬ時に受ける最後のダメージ（アニメーションなどの関係で設定）
	bool m_isLastDamaged = false;

	// 描画位置の調整（shartelで使う）
	int m_adjShartelSlashPosX = 0;

public:
	Player();
	virtual ~Player();
	void Init(float _posX, int _pIdx);
	void SecondInit(float _posX);
	void Update(const Pad& _pad, bool isFinishGame);
	/// <summary>
	/// プレイヤーのポジションを更新(押し戻しとかの関係で追加)
	/// </summary>
	void UpdateMove(bool _isStart);
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	/// <summary>
	/// Xのスクロール座標を設定
	/// </summary>
	void SetScrollX(float _scrollX);
	/// <summary>
	/// 現在の行動状態を取得
	/// </summary>
	/// <returns>currentState</returns>
	StateID GetCurrentAction(const Pad& _pad);
	/// <summary>
	/// プレイヤーの中心ポジションを取得
	/// </summary>
	Vec2 GetPos() const { return m_pos; };
	void SetPos(Vec2 _pos);
	/// <summary>
	/// プレイヤーの中心座標を取得
	/// </summary>
	/// <returns></returns>
	Vec2 GetCenterPos() const;
	/// <summary>
	/// プレイヤーの押し戻し判定の右端の座標を取得（押し戻しに使用）
	/// </summary>
	/// <returns>押し戻しの右端</returns>
	float GetPBRightEdge() const;
	/// <summary>
	/// プレイヤーの押し戻し判定の左端の座標を取得（押し戻しに使用）
	/// </summary>
	/// <returns>押し戻しの左端</returns>
	float GetPBLeftEdge() const;
	/// <summary>
	/// プレイヤーの押し戻し判定の上端の座標を取得（押し戻しに使用）
	/// </summary>
	/// <returns></returns>
	float GetPBTopEdge() const;
	/// <summary>
	/// プレイヤーが右画面端にいるかどうかを示す
	/// </summary>
	/// <returns>プレイヤーが画面端にいるならTrue</returns>
	bool GetIsRightEdge() const;
	/// <summary>
	/// プレイヤーが左画面端にいるかどうかを示す
	/// </summary>
	/// <returns>プレイヤーが画面端にいるならTrue</returns>
	bool GetIsLeftEdge() const;
	/// <summary>
	/// プレイヤーの仮想の動きを取得
	/// </summary>
	/// <returns>＋なら前、-なら後ろ方向</returns>
	float GetMoveX() const { return m_move.x * m_dir; };
	/// <summary>
	/// プレイヤーの仮想の動きを取得
	/// </summary>
	/// <returns>＋なら前、-なら後ろ方向</returns>
	float GetVirtualMoveX() const { return m_virtualMove.x * m_dir; };
	/// <summary>
	/// プレイヤーのY方向の動きを取得
	/// </summary>
	/// <returns></returns>
	float GetMoveY() const { return m_move.y; };
	Vec2 GetMove() const { return m_move; };
	/// <summary>
	/// 地上にいるかどうか
	/// </summary>
	/// <returns>地上にいるならTRUE</returns>
	bool GetIsLand() const { return m_isLanding; };
	/// <summary>
	/// 波動状態を取得
	/// </summary>
	/// <returns></returns>
	bool GetIsHadou() const { return m_isHadou; }
	/// <summary>
	/// 右向き左向きの設定
	/// </summary>
	/// <param name="_rightLeft">左向き=‐1、右向き＝1</param>
	void SetRightLeft(int _rightLeft);
	/// <summary>
	/// 右向き左向きを取得
	/// </summary>
	/// <returns>左向き=‐1、右向き＝1</returns>
	int GetRightLeft() const { return m_dir; };
	/// <summary>
	/// 
	/// </summary>
	/// <param name="_col"></param>
	/// <returns></returns>
	std::vector<Rect> GetCol(ColType _type) const;
	/// <summary>
	/// 攻撃が当たった時の処理
	/// </summary>
	void SetIsHit(bool _hit);
	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	/// <param name="_damage">与えるダメージ量</param>
	void IsDamaged(int _damage, AttackType _type);
	/// <summary>
	/// 攻撃のメイン当たり判定のダメージ
	/// </summary>
	/// <returns></returns>
	int GetMainDamage() const { return m_mainDamage; };
	/// <summary>
	/// ガードをするときの処理
	/// </summary>
	/// <param name="_damage">受けたダメージ量</param>
	void IsGuard(int _damage);
	/// <summary>
	/// 攻撃の当たり判定をリセットする(着地後などでつかう)
	/// </summary>
	void DeactiveHitCol();
	/// <summary>
	/// 押し戻し状態かどうかを設定する
	/// </summary>
	void SetIsPushBack(PushBackType _pushBack);
	/// <summary>
	/// 一度だけ当てるための処理
	/// </summary>
	/// <returns>当たったかどうか</returns>
	bool GetIsHitOnce() const { return m_isHitOnce; };
	/// <summary>
	/// 攻撃の強さを取得する
	/// </summary>
	/// <returns>attackType</returns>
	AttackType GetAttackType() const { return m_attackScale; };
	/// <summary>
	/// プレイヤーのHPを取得
	/// </summary>
	/// <returns>HP</returns>
	int GetPlayerHP() const { return m_hp; };
	/// <summary>
	/// プレイヤーの最大HPを取得
	/// </summary>
	/// <returns>MaxHP</returns>
	int GetPlayerMaxHP() const;
	/// <summary>
	/// ヒットバックを設定
	/// </summary>
	/// <param name="_isHitBack"></param>
	void SetIsHitBack(bool _isHitBack) { m_isHitBack = _isHitBack; };

protected:
	virtual StateID InputAction(const Pad& _pad) = 0;
	/// <summary>
	/// アニメーションのハンドルやフレーム数などの情報をセット
	/// </summary>
	virtual void SetAnimData() = 0;
	/// <summary>
	/// 当たり判定の情報をセット
	/// </summary>
	virtual void SetColData() = 0;
	/// <summary>
	/// 現在のアニメーションや動きを設定する
	/// </summary>
	virtual void SetCurrentAction(StateID _state) = 0;
	/// <summary>
	/// アクションの動きを変えたり、フレーム指定でアクションに変更を加える
	/// </summary>
	virtual void SetActionDetail() = 0;
	/// <summary>
	/// 攻撃など一時的に生成する当たり判定の処理
	/// </summary>
	virtual void SetAttackCol() = 0;
	/// <summary>
	/// 当たり判定
	/// </summary>
	void UpdateCollision();
	/// <summary>
	/// 行動の管理を行う
	/// </summary>
	void ActionManager(const Pad& _pad);
	/// <summary>
	/// 重力の処理を行う
	/// </summary>
	void Gravity();
	/// <summary>
	/// アニメーションの遷移を行う
	/// </summary>
	/// <param name="_nextID"></param>
	void ChangeAnim(AnimID _nextID);
	/// <summary>
	/// 行う行動の情報を設定する
	/// </summary>
	/// <param name="_moveX">X方向のスピード</param>
	/// <param name="_moveY">Y方向のスピード</param>
	/// <param name="_coolDown">クールダウン</param>
	/// <param name="_action">キャンセルのできないアクションかどうか</param>
	/// <param name="_cdAnim">CD時にfalse :アニメーションの最後のコマでとめる、true :IdleAnimへ遷移</param>
	void SetActionInfo(float _moveX,float _moveY, int _coolDown, bool _action, bool _cdAnim);
	/// <summary>
	/// コリジョンをセットする
	/// </summary>
	/// <param name="_box">Box</param>
	/// <param name="_col">Col</param>
	void SetCol(Box _box, Rect& _col);
	/// <summary>
	/// プレイヤーの体当たり判定の状態を変更する
	/// </summary>
	/// <param name="_col">コリジョンの種類</param>
	void ChangeColType(ColType _col);
	
	void GeneCol(ColType _col, ColDetail _colDetail, int _animIdxMin, int _animIdxMax, int _frameMin, int _frameMax);
	/// <summary>
	/// フレームを指定し、コリジョンのBoxを変更する（主に攻撃で使う）
	/// </summary>
	/// <param name="_col"></param>
	/// <param name="_colDetail"></param>
	/// <param name="_animIdxMin"></param>
	/// <param name="_animIdxMax"></param>
	/// <param name="_frameMin"></param>
	/// <param name="_frameMax"></param>
	/// <param name="_box"></param>
	void SetFrameColBox(ColType _col, ColDetail _colDetail, int _animNum, int _animFrame, Box _box);
	/// <summary>
	/// 当たり判定のボックスの設定
	/// </summary>
	/// <param name="_col"></param>
	/// <param name="_colDetail"></param>
	/// <param name="_box"></param>
	void SetColBox(ColType _col, ColDetail _colDetail, Box _box);
	/// <summary>
	/// 頭体足の当たり判定を設定できる
	/// </summary>
	/// <param name="_col"></param>
	/// <param name="_body"></param>
	/// <param name="_head"></param>
	/// <param name="_leg"></param>
	void SetBoxBodyHead(ColType _col, Box _body, Box _head, Box _leg, Box _arm);

	void ResetAttackCol();

	void PlayHitSE();

	void PlayGuardSE();

	void PlayWhiffSE();
};

