#include "PointUI.h"
#include "DxLib.h"

namespace
{
	constexpr int kMaxPoint = 2;
	constexpr int kDistance = 32;
	constexpr int kGraficSize = 64;
}

PointUI::PointUI()
{}

PointUI::~PointUI()
{
	DeleteGraph(m_starHandle);
	DeleteGraph(m_starBackHandle);
}

void PointUI::Init()
{
	m_starHandle = LoadGraph("UI/star.png");
	m_starBackHandle = LoadGraph("UI/starBack.png");
}

void PointUI::Update(int _point)
{
	m_point = _point;
}

void PointUI::Draw(int _posX, int _posY)
{
	int pointHandle[kMaxPoint] = { m_starBackHandle, m_starBackHandle };
	if (m_point >= 1)
	{
		pointHandle[0] = m_starHandle;
	}
	
	if (m_point >= 2)
	{
		pointHandle[1] = m_starHandle;
	}

	float width = static_cast<float>(kGraficSize * 2 + kDistance);
	int centerX = _posX - static_cast<int>(width * 0.5f);
	int drawX = centerX;

	for (int i = 0; i < kMaxPoint; i++)
	{
		DrawGraph(drawX + (kDistance + kGraficSize) * i, _posY,pointHandle[i], true);
	}
}
