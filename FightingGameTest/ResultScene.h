#pragma once
#include "SceneBase.h"
#include "WinChara.h"

struct WinnerData
{
	Chara chara;
	int playerNum;
};

class ResultScene : public SceneBase
{
private:
	WinnerData m_winner = { Chara::Nova, 0 };

	std::unique_ptr<WinChara> m_winChara = nullptr;

	int m_p1WinHandle = 0;
	int m_p2WinHandle = 0;
	int m_winnerHandle = 0;
	int m_buttonAHandle = 0;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResultScene();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ResultScene();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init(GameData& data) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update(GameData& data, const Pad& _pad, const Pad& _pad2) override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(GameData& data) override;
};