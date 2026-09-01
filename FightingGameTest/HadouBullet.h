#pragma once
#include "Vec2.h"
#include "Animation.h"
#include "Rect.h"

class HadouBullet
{
private:
	Vec2 m_pos = { 0, 0 };
	// trueなら左向き、falseなら右向き
	int m_dir = 0;
	int m_handle = 0;
	Animation m_anim;
	Rect m_col;
	int m_damage = 0;
	

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	HadouBullet();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~HadouBullet();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_posX">x座標</param>
	/// <param name="_posY">y座標</param>
	/// <param name="_isTurn">左向き(true)、右向き(false)</param>
	void Init(float _posX, float _posY, int _dir);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// HadouのX座標を取得する
	/// </summary>
	Vec2 GetPos();
	/// <summary>
	/// 当たり判定を取得
	/// </summary>
	/// <returns></returns>
	Rect GetCol() const { return m_col; };
	/// <summary>
	/// ダメージを取得
	/// </summary>
	/// <returns></returns>
	int GetDamage() const;
};

