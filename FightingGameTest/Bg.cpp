#include "Bg.h"

namespace
{
	constexpr int kPixelSize = 100;
	constexpr int kPixelNumX = (Game::kScreenWidth / kPixelSize) * 2;
	constexpr int kPixelNumY = (Game::kScreenHeight / kPixelSize) * 2;
	// スクロール
	constexpr float kScrollSpeed[static_cast<int>(Beach::Max)] =
	{
		0.25f,//Sky
		0.25f,//Horizon
		0.4f,//Ocean
		0.5f,//Sand
		0.7f,//Grass
		1.0f//Ground
	};
	constexpr int kBgWidth = 1600;
	constexpr int kBgMaxNum = 4;

	// カニ
	constexpr int kCrabTurnFrame = 180;
	constexpr float kCrabSpeed = 0.5f;
}

Bg::Bg()
{
}

Bg::~Bg()
{
	// メモリの開放
	for (int i = 0; i < static_cast<int>(Beach::Max); i++)
	{
		DeleteGraph(beachHandle[i]);
	}
}

void Bg::Init()
{
	// ビーチ背景の画像ハンドルを取得
	beachHandle[static_cast<int>(Beach::Grass)] = LoadGraph("bg/beach/grass.png");
	beachHandle[static_cast<int>(Beach::Ground)] = LoadGraph("bg/beach/ground.png");
	beachHandle[static_cast<int>(Beach::Horizon)] = LoadGraph("bg/beach/horizon.png");
	beachHandle[static_cast<int>(Beach::Ocean)] = LoadGraph("bg/beach/ocean.png");
	beachHandle[static_cast<int>(Beach::Sand)] = LoadGraph("bg/beach/sand.png");
	beachHandle[static_cast<int>(Beach::Sky)] = LoadGraph("bg/beach/sky.png");

	// カニの画像ハンドルを取得
	m_crabHandle = LoadGraph("bg/beach/crabWalk.png");
	m_crabWalk.Init(m_crabHandle, 10, 4, true);
	m_crabMove = kCrabSpeed;

	m_beachPos[static_cast<int>(Beach::Ground)].y = 100.0f;
	m_beachPos[static_cast<int>(Beach::Grass)].y = 50.0f;
}

void Bg::SecondInit()
{
	// 2度目以降の初期化
	m_pos = { Game::kStageLeft, 0 };
	for (int i = 0; i < static_cast<int>(Beach::Max); i++)
	{
		m_beachPos[i].x = 0;
	}
}

void Bg::Update()
{
	
	CrabUpdate();
	m_pos.x += m_scrollX;
	m_stage.left -= m_scrollX;
	m_stage.right -= m_scrollX;
	for (int i = 0; i < static_cast<int>(Beach::Max); i++)
	{
		m_beachPos[i].x += m_scrollX * kScrollSpeed[i];
		if (i == static_cast<int>(Beach::Sand))
		{
			m_crabPos += m_scrollX * kScrollSpeed[i] + m_crabMove;
		}
		if (i == static_cast<int>(Beach::Sky))
		{
			m_beachPos[i].x += 0.1f;
		}
		else if (i == static_cast<int>(Beach::Ocean))
		{
			m_beachPos[i].x += 0.3f;
		}
	}
}

void Bg::Draw()
{
	// ビーチ背景の描画
	for (int i = 0; i < static_cast<int>(Beach::Max); i++)
	{
		for (int j = 0; j < kBgMaxNum; j++)
		{
			DrawGraph(static_cast<int>((kBgWidth * j - kBgWidth * kBgMaxNum * 0.5f) + m_beachPos[i].x), static_cast<int>(m_pos.y + m_beachPos[i].y), beachHandle[i], true);
			if (i == static_cast<int>(Beach::Sand))
			{
				// カニの描画
				for (int k = 0; k < 3; k++)
				{
					m_crabWalk.Draw(m_crabPos + k * 100, 500, false);
				}
				//DrawGraph(m_crabPos, 500, m_crabHandle, true);
			}
		}
	}

#ifdef _DEBUG
	// 仮でグリッドを描画する
	// 1画面の2倍描画することとする
	for (int x = 0; x < kPixelNumX; x++)
	{
		for (int y = 0; y < kPixelNumY; y++)
		{
			int left = static_cast<int>(m_pos.x + kPixelSize * x);
			int top = static_cast<int>(m_pos.y + kPixelSize * y);
			DrawBox(left, top, left + kPixelSize, top + kPixelSize, 0xeeeeee, false);
		}
	}

	/*DrawFormatString(200, 200, 0xff0000, "right: %f", m_stage.right);
	DrawFormatString(200, 100, 0xff0000, "left: %f", m_stage.left);*/
#endif
}

void Bg::DrawFront()
{
	for (int j = 0; j < kBgMaxNum; j++)
	{
		int idx = static_cast<int>(Beach::Ground);
		DrawGraph(static_cast<int>((kBgWidth * j - kBgWidth * kBgMaxNum * 0.5f) + m_beachPos[idx].x), static_cast<int>(m_pos.y + m_beachPos[idx].y), beachHandle[idx], true);
	}
}

void Bg::SetScrollX(float _scrollX)
{
	m_scrollX = _scrollX;
}

void Bg::CrabUpdate()
{
	m_crabTimer++;
	if (m_crabTimer > kCrabTurnFrame)
	{
		m_crabTimer = 0;
		m_crabMove *= -1;
	}
	m_crabWalk.Update();
}
