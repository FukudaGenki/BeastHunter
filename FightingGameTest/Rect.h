#pragma once
#include "Vec2.h"

/// <summary>
/// 矩形を管理するクラス
/// </summary>
class Rect
{
public:
	float m_left = 0.0f;	// 左上のX座標
	float m_top = 0.0f;	// 左上のY座標
	float m_right = 0.0f;	// 右下のX座標
	float m_bottom = 0.0f;	// 右下のY座標
private:
	bool m_offset = true; // 当たり判定の非アクティブ状態

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Rect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Rect();

	/// <summary>
	/// 矩形の描画
	/// </summary>
	/// <param name="color">矩形の色</param>
	/// <param name="isFill">太さ</param>
	void Draw(unsigned int Color, bool isFill);

	/// <summary>
	/// 左上座標と幅高さを指定
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="top">上</param>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void SetLT(float left, float top, float width, float height);

	/// <summary>
	/// 左下座標と幅高さを指定
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="bottom">下</param>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void SetLB(float left, float bottom, float width, float height);

	/// <summary>
	/// 中心座標と幅高さを指定
	/// </summary>
	/// <param name="x">中心のX座標</param>
	/// <param name="y">中心のY座標</param>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void SetCenter(float x, float y, float width, float height);

	/// <summary>
	/// 矩形の幅を取得
	/// </summary>
	/// <returns>矩形の幅</returns>
	float GetWidth() const;

	/// <summary>
	/// 矩形の高さを取得
	/// </summary>
	/// <returns>矩形の高さ</returns>
	float GetHeight() const;

	/// <summary>
	/// 矩形の中心座標を取得
	/// </summary>
	/// <returns>矩形の中心座標</returns>
	Vec2 GetCenter() const;

	/// <summary>
	/// 矩形の左上のX座標を取得
	/// </summary>
	/// <returns>左上のX座標</returns>
	float GetLeft() const { return m_left; }

	/// <summary>
	/// 矩形の左上のY座標を取得
	/// </summary>
	/// <returns>左上のY座標</returns>
	float GetTop() const { return m_top; }

	/// <summary>
	/// 矩形の右下のX座標を取得
	/// </summary>
	/// <returns>右下のX座標</returns>
	float GetRight() const { return m_right; }

	/// <summary>
	/// 矩形の右下のY座標を取得
	/// </summary>
	/// <returns>矩形の右下のY座標</returns>
	float GetBottom() const { return m_bottom; }

	/// <summary>
	/// 矩形の当たり判定
	/// </summary>
	/// <param name="rect">矩形情報</param>
	/// <returns>当たってるかどうか</returns>
	bool IsCollision(const Rect& rect);

	void OffSet();

	/// <summary>
	/// 矩形があたっているときの当たっている領域の中心を取得する
	/// </summary>
	Vec2 GetColCenterPos(const Rect& rect);
};

