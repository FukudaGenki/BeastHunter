#pragma once
#include "Vec2.h"
#include "Player.h"
#include "GameDefine.h"
#include "GameManager.h"
#include "GameData.h"
#include "TimeUIManager.h"
#include "HPUIManager.h"
#include "PointUI.h"
#include <memory>

/*struct HpBarData
{
	int pIdx;
	int hp;
};*/

struct BoxData
{
	int x1;
	int y1;
	int x2;
	int y2;
};

class UIManager
{
private:
	Player* m_p1 = nullptr;
	Player* m_p2 = nullptr;
	GameManager* m_gm = nullptr;
	std::unique_ptr<TimeUIManager> m_Time = nullptr;
	std::unique_ptr<HPUIManager> m_HpBar = nullptr;
	std::unique_ptr<PointUI> m_P1Point = nullptr;
	std::unique_ptr<PointUI> m_P2Point = nullptr;
	
	// 画像ハンドル
	int m_readyHandle = 0;
	int m_fightHandle = 0;
	int m_p1WinHandle = 0;
	int m_p2WinHandle = 0;
	int m_KOHandle = 0;
	int m_hpBarHandle = 0;
	int m_hpBarOutLineHandle = 0;

	// HP
	int m_p1MaxHp = 0;
	int m_p2MaxHp = 0;

	float m_p1HpPer = 1.0f;
	float m_p2HpPer = 1.0f;

	int m_WhiteOutFrameTimer = 0;
	int m_KOFrameTimer = 0;
	int m_whiteOutIdx = 0;
	// 始まるとき
	int m_startFrameTimer = 0;

	GameData m_data = {};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UIManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UIManager();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init(const std::unique_ptr<Player>& _p1, const std::unique_ptr<Player>& _p2, const std::unique_ptr<GameManager>& _gm);
	void SecondInit();
	/// <summary>
	/// 更新
	/// </summary>
	void Update(GameData& _data);
	/// <summary>
	/// HPの更新
	/// </summary>
	void UpdateHp();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	void DrawFront();
	/// <summary>
	/// HPバーの描画
	/// </summary>
	void DrawHpBar();
	void FinishPhase();
	void FinishPhaseFront();
	void StartGame();
};