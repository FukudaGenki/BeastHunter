#include "DxLib.h"
#include <algorithm>
#include "Rect.h"
#include <cassert>
#include <cmath>
#include "Vec2.h"

Rect::Rect()
{

}

Rect::~Rect()
{

}

void Rect::Draw(unsigned int color, bool isFill)
{
	if (!m_offset)
	{
		DrawBox(static_cast<int>(m_left), static_cast<int>(m_top), static_cast<int>(m_right), static_cast<int>(m_bottom), color, isFill);
	}
}

void Rect::SetLT(float left, float top, float width, float height)
{
	m_offset = false;
	m_left = left;
	m_top = top;
	m_right = left + width;
	m_bottom = top + height;
}

void Rect::SetLB(float left, float bottom, float width, float height)
{
	m_offset = false;
	m_left = left;
	m_top = bottom - height;
	m_right = left + width;
	m_bottom = bottom;
}

void Rect::SetCenter(float x, float y, float width, float height)
{
	m_offset = false;
	m_left = x - width / 2;
	m_top = y - height / 2;
	m_right = x + width / 2;
	m_bottom = y + height / 2;
}

float Rect::GetWidth() const
{
	assert(m_right >= m_left);	// 左右の座標入れ替わりチェック
	return m_right - m_left;
}

float Rect::GetHeight() const
{
	assert(m_bottom >= m_top);	// 上下の座標入れ替わりチェック
	return m_bottom - m_top;
}

Vec2 Rect::GetCenter() const
{
	// 中心座標
	float x = (m_left + m_right) / 2;
	float y = (m_top + m_bottom) / 2;
	return Vec2{ x,y };
}

bool Rect::IsCollision(const Rect& rect)
{
	// 非アクティブなら判定しない
	if (m_offset) return false;
	// 絶対に当たらないパターンをはじいていく
	if (m_left > rect.m_right) return false;
	if (m_top > rect.m_bottom) return false;
	if (m_right < rect.m_left) return false;
	if (m_bottom < rect.m_top) return false;

	// 当たらないパターン以外は当たっている
	return true;
}

// colの状態を０にする
void Rect::OffSet()
{
	m_offset = true;
	m_left = 0;
	m_top = 0;
	m_right = 0;
	m_bottom = 0;
}

Vec2 Rect::GetColCenterPos(const Rect& rect)
{
	if (!IsCollision(rect))
	{
		return { 0.0f, 0.0f };
	}

	// 重なっている領域の四隅を取得する
	// 左端
	float x1 = (std::max)(m_left, rect.m_left);
	// 右端
	float x2 = (std::min)(m_right, rect.m_right);
	// 上端
	float y1 = (std::max)(m_top, rect.m_top);
	// 下端
	float y2 = (std::min)(m_bottom, rect.m_bottom);

	// 重なっている領域の中心座標を返す
	return Vec2{ (x1 + x2) * 0.5f, (y1 + y2) * 0.5f };
	/*
	if (IsCollision(rect))
	{
		// 当たっているとき
		// どのように重なっているのか取得
		// 左側が重なっている
		bool left = m_left <= rect.m_right;
		// 右側が重なっている
		bool right = m_right >= rect.m_left;
		// 上側が重なっている
		bool top = m_top <= rect.m_bottom;
		// 下側が重なっている
		bool bottom = m_bottom >= rect.m_top;
		// 重なっている領域の四角形の四隅の座標を取得する
		float x1 = 0;
		float x2 = 0;
		float y1 = 0;
		float y2 = 0;

		// X座標
		if (left && right)
		{
			// 自分が内側にいるかどうかを取得する
			bool inside = m_left > rect.m_left && m_right < rect.m_right;
			// 自分が内側なら自分の中心X座標でよい
			if (inside)
			{
				x1 = m_left;
				x2 = m_right;
			}
			else if (!inside)
			{
				x1 = rect.m_left;
				x2 = rect.m_right;
			}
		}
		else if (left)
		{
			x1 = m_left;
			x2 = rect.m_right;
		}
		else if (right)
		{
			x1 = rect.m_left;
			x2 = m_right;
		}

		// Y座標
		if (top && bottom)
		{
			// 自分が内側にいるかどうかを取得する
			bool inside = m_top > rect.m_top && m_bottom < rect.m_bottom;
			// 自分が内側なら自分の中心X座標でよい
			if (inside)
			{
				y1 = m_top;
				y2 = m_bottom;
			}
			else if (!inside)
			{
				y1 = rect.m_top;
				y2 = rect.m_bottom;
			}
		}
		else if (top)
		{
			y1 = m_top;
			y2 = rect.m_bottom;
		}
		else if (bottom)
		{
			y1 = rect.m_top;
			y2 = m_bottom;
		}
		
		// 重なっている領域の上下左右から中心の座標を求める
		Vec2 center = {};
		center.x = (x1 + x2) * 0.5f;
		center.y = (y1 + y2) * 0.5f;
		return center;
	}
	else
	{
		// 当たってないとき
		return { 0, 0 };
	}*/
}
