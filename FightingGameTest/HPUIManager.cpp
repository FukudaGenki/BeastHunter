#include <DxLib.h>
#include "HPUIManager.h"

namespace
{
	constexpr int kPosX = 0;
	constexpr int kPosY = 30;
}

HPUIManager::HPUIManager()
{
	
}

HPUIManager::~HPUIManager()
{
	DeleteGraph(m_hpBarHandle);
	DeleteGraph(m_hpBarOutlineHandle);
}

void HPUIManager::Init(int _p1MaxHp, int _p2MaxHp)
{
	m_hpBarHandle = LoadGraph("UI/HP/hpbar.png");
	m_hpBarOutlineHandle = LoadGraph("UI/HP/hpbarOutline0.png");

	// 最大HPを設定する
	m_p1MaxHp = _p1MaxHp;
	m_p2MaxHp = _p2MaxHp;

	// 画像の大きさ取得、HPバー本体の画像サイズを取得
	GetGraphSize(m_hpBarHandle, &m_sizeX, &m_sizeY);

	m_p1BarPos =
	{
		kPosX,
		kPosY,
	};

	m_p2BarPos =
	{
		Game::kScreenWidth - (kPosX + m_sizeX),
		kPosY
	};
}

void HPUIManager::Update(int _p1Hp, int _p2Hp)
{
	m_p1HpPer = static_cast<float>(_p1Hp) / static_cast<float>(m_p1MaxHp);
	// HPが0より小さくなった時０とする
	if (m_p1HpPer < 0.0f)
	{
		m_p1HpPer = 0;
	}

	m_p2HpPer = static_cast<float>(_p2Hp) / static_cast<float>(m_p2MaxHp);
	// HPが0より小さくなった時０とする
	if (m_p2HpPer < 0.0f)
	{
		m_p2HpPer = 0;
	}
}

void HPUIManager::Draw()
{
	PosData p1 = m_p1BarPos;
	PosData p2 = m_p2BarPos;

	// HPバーの本体に若干の空白があり、正しくHPの量を表示できないため、
	// 左右端の空白部分を切り抜いて大きさを調整する必要がある
	// 切り取る大きさ
	float slicePer = 0.05f;
	float sliceOneSidePer = slicePer * 0.5f;
	// 切り取って残した分の大きさ
	float remainderPer = 1.0f - slicePer;

	// 描画の左上座標の位置を調整する変数
	int posAdjX = static_cast<int>(m_sizeX * sliceOneSidePer);

	// 切り取り左上の座標（P1はここを切り取る）
	//int p1SrcX = static_cast<int>(m_sizeX * sliceOneSidePer - m_sizeX * (1.0f - m_p1HpPer));
	int p1SrcX = static_cast<int>(m_sizeX * sliceOneSidePer);
	int p2SrcX = static_cast<int>(m_sizeX * sliceOneSidePer);
	int srcY = 0;

	// 横幅（P2はここを切り取る）
	//int p1SizeX = static_cast<int>(remainderPer * m_sizeX);
	int p1SizeX = static_cast<int>(remainderPer * (m_sizeX - m_sizeX * (1.0f - m_p1HpPer)));
	int p2SizeX = static_cast<int>(remainderPer * (m_sizeX - m_sizeX * (1.0f - m_p2HpPer)));

	// プレイヤー１は左から減ってるように見せるために左側の座標をずらす必要がある
	int p1SliceSizeX = static_cast<int>(remainderPer * (m_sizeX - m_sizeX * m_p1HpPer));

	// Player1
	// 外枠
	DrawRectGraph
	(
		p1.x, p1.y, // 描画する座標
		0, 0, // 切り取り左上座標
		m_sizeX, m_sizeY, // 画像サイズ
		m_hpBarOutlineHandle, // 画像ハンドル
		true,
		true
	);
	// 本体
	DrawRectGraph
	(
		p1.x + p1SliceSizeX + posAdjX, p1.y, // 描画する座標
		p1SrcX, srcY, // 切り取り左上座標
		p1SizeX , m_sizeY, // 画像サイズ
		m_hpBarHandle, // 画像ハンドル
		true,
		true
	);

	// Player2
	// 外枠
	DrawGraph(p2.x, p2.y, m_hpBarOutlineHandle, true);
	// 本体
	DrawRectGraph
	(
		p2.x + posAdjX, p2.y, // 描画する座標
		p2SrcX, srcY, // 切り取り左上座標
		p2SizeX, m_sizeY, // 画像サイズ
		m_hpBarHandle,
		true,
		false
	);
}
