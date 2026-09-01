#include "DxLib.h"
#include "TimeUIManager.h"

namespace
{
	// スプライトシートの画像の総数
	constexpr int kTotalGraphNum = 10;
	// カウントの最大数
	constexpr int kMaxCount = 99;
	// 1カウントあたりに必要なフレーム数
	constexpr int kCountFrame = 60;
	// 1桁目の画像を表示する場所
	constexpr Vec2 kPos = { 778, 0 };
	// 1桁目と2桁目の距離
	constexpr int kDistPos = 90;
}

TimeUIManager::TimeUIManager()
{
}

TimeUIManager::~TimeUIManager()
{
	DeleteGraph(m_numberHandle);
}

void TimeUIManager::Init()
{
	m_numberHandle = LoadGraph("UI/number.png");
	int totalSizeX = 0;
	int sizeY = 0;
	GetGraphSize(m_numberHandle, &totalSizeX, &sizeY); // 画像の全体サイズを取得

	m_sizeX = totalSizeX / kTotalGraphNum; // 1コマあたりの画像サイズを取得
	m_sizeY = sizeY; // ここは変わらない（一直線だから）
}

void TimeUIManager::Update(int _frame)
{
	// フレームから現在のカウントに変換して最大秒数から引く
	int totalCount = kMaxCount - _frame / kCountFrame;
	// 一桁目
	int onceDigedCount = totalCount % 10;
	// 二桁目
	int secondDigedCount = (totalCount % 100) / 10;
	// 代入
	m_currentCount[0] = onceDigedCount;
	m_currentCount[1] = secondDigedCount;
}

void TimeUIManager::Draw()
{
	for (int i = 0; i < 2; i++)
	{
		// 表示座標のX,Y
		int posX = static_cast<int>(kPos.x) - kDistPos * i;
		int posY = static_cast<int>(kPos.y);
		// 切り取りとる左上のX、Y
		int scrX = m_sizeX * m_currentCount[i];
		int scrY = 0;

		// 描画処理
		DrawRectGraph
		(
			posX, posY,  // 描画する座標
			scrX, scrY, // 切り取る座標の左上
			m_sizeX, m_sizeY, // 切り取る画像の大きさ
			m_numberHandle, // 画像ハンドル
			true, // 透明度の有無
			false // 反転の有無
		);
	}
}
