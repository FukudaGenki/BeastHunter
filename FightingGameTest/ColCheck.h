#pragma once
#include <memory>
#include <array>
#include "Player.h"
#include "HadouBullet.h"

struct IsColData
{
	bool isCol;
	Vec2 colPos;
};

class ColCheck
{
private:
	Player* m_p1 = nullptr;
	Player* m_p2 = nullptr;
	// ヒット、ガードしたかどうかのフラグ
	
	IsColData m_p1Hit = {};
	IsColData m_p1Guard = {};
	IsColData m_p2Hit = {};
	IsColData m_p2Guard = {};

public:
	ColCheck();
	virtual ~ColCheck();
	void Init(const std::unique_ptr<Player>& _p1, const std::unique_ptr<Player>& _p2);
	void Update();
	// ヒット、ガードしたかどうかを取得
	IsColData GetP1Hit() const { return m_p1Hit; };
	IsColData GetP1Guard()const { return m_p1Guard; };
	IsColData GetP2Hit() const { return m_p2Hit; };
	IsColData GetP2Guard() const { return m_p2Guard; };
	Vec2 GetEffectPos();
private:
	IsColData IsCol1PTo2P(ColType _type1, ColType _type2);
	IsColData IsCol2PTo1P(ColType _type1, ColType _type2);
	void SetColCenterPos();
};