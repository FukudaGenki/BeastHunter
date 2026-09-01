#pragma once
#include <DxLib.h>
#include "Vec2.h"
#include "GameDefine.h"
#include "Animation.h"

struct Stage
{
	float left;
	float right;
};

/*enum class Beach
{
	Grass,
	Ground,
	Horizon,
	Ocean,
	Sand,
	Sky,
	Max
};*/

enum class Beach
{
	Sky,
	Horizon,
	Ocean,
	Sand,
	Grass,
	Ground,
	Max
};

class Bg
{
private:
	int beachHandle[static_cast<int>(Beach::Max)] = {};
	Vec2 m_beachPos[static_cast<int>(Beach::Max)] = {};
	Vec2 m_beachSkyPos[3] = {-1600, 0, 1600};
	Vec2 m_pos = {Game::kStageLeft, 0};
	float m_scrollX = 0;
	Stage m_stage = { 0, Game::kScreenWidth };

	// カニ
	Animation m_crabWalk;
	int m_crabHandle = 0;
	float m_crabPos = 0;
	int m_crabTimer = 0;
	float m_crabMove = 0;

public:
	Bg();
	virtual ~Bg();
	void Init();
	void SecondInit();
	void Update();
	void Draw();
	void DrawFront();
	void SetScrollX(float _scrollX);
	Stage GetStagePos() const { return m_stage; };
private:
	void CrabUpdate();
};

