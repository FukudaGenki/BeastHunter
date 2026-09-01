#include "HadouBullet.h"
#include "DxLib.h"
// 中心座標基準

namespace
{
	constexpr float kBulletSpeed = 15.0f;
	constexpr float kGraficSize = 96.0f;
	constexpr float kColSize = 70.0f;
	constexpr int kDamage = 50;
}

HadouBullet::HadouBullet()
{
	
}

HadouBullet::~HadouBullet()
{
	// 画像ハンドルはAnimationクラスから自動で削除される
}

void HadouBullet::Init(float _posX, float _posY, int _dir)
{
	// ポジションの設定
	m_pos.x = _posX;
	m_pos.y = _posY;
	// 反転するかどうか
	m_dir = _dir;
	// アニメーションの設定
	m_handle = LoadGraph("effect/hadou.png");
	m_anim.Init(m_handle, 10, 4, true);
	m_damage = kDamage;
}

void HadouBullet::Update()
{
	// 移動
	m_pos.x += kBulletSpeed * m_dir;
	// アニメーション
	m_anim.Update();
	// 当たり判定
	Vec2 centerPos = { m_pos.x + kGraficSize * 0.5f, m_pos.y + kGraficSize * 0.5f };
	m_col.SetCenter(centerPos.x, centerPos.y, kColSize, kColSize);
}

void HadouBullet::Draw()
{
	if (m_dir == 1)
	{
		m_anim.DrawCenter(m_pos.x, m_pos.y, false);
	}
	else if(m_dir == -1)
	{
		m_anim.DrawCenter(m_pos.x, m_pos.y, true);
	}
#ifdef _DEBUG
	m_col.Draw(0xff0000, false);
#endif
}

Vec2 HadouBullet::GetPos()
{
	return m_pos;
}

int HadouBullet::GetDamage() const
{
	return m_damage;
}
