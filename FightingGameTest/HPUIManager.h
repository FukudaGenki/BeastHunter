#pragma once
#include "GameDefine.h"

struct HpBarData
{
	int pIdx;
	int hp;
};

struct PosData
{
	int x;
	int y;
};

class HPUIManager
{
private:
	// HPの更新処理関連
	int m_p1MaxHp = 0;
	int m_p1CurrentHp = 0;
	int m_p2MaxHp = 0;
	int m_p2CurrentHp = 0;
	float m_p1HpPer = 1;
	float m_p2HpPer = 1;

	// P1のHPバーアウトラインのデータ
	PosData m_p1BarPos = {};
	// P2のHPバーアウトラインのデータ
	PosData m_p2BarPos = {};

	// 描画処理関連
	// HPバー本体の画像ハンドル
	int m_hpBarHandle = 0;
	// HPバーのアウトラインの画像ハンドル
	int m_hpBarOutlineHandle = 0;
	// 画像の全体サイズ
	int m_sizeX = 0;
	// 画像の全体サイズ
	int m_sizeY = 0;
public:
	HPUIManager();
	virtual ~HPUIManager();
	void Init(int _p1MaxHp, int _p2MaxHp);
	void Update(int _p1Hp, int _p2Hp);
	void Draw();
};