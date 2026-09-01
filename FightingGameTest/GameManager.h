#pragma once
#include <memory>
#include "Player.h"
#include "GameData.h"

class GameManager
{
private:
	Player* m_p1 = nullptr;
	Player* m_p2 = nullptr;
	// 0->なにもなし 3->ダブルKO
	int m_winPlayer = 0;
	bool m_isFinish = false;
	// 次の試合までの時間を数えるタイマー
	int m_nextPhaseFrameTimer = 0;
	bool m_nextPhase = false;
	// ポイントの管理
	int m_p1Point = 0;
	int m_p2Point = 0;
	// ポイントを足したかどうかのフラグ
	bool m_isPlusPoint = false;
	// レディファイトみたいなのを表示している間はするときは動かさないようにするのでそれ用の変数
	bool m_isStart = false;
	int m_startFrameTimer = false;
	// 2本先取したかどうか
	bool m_is2Point = false;
	// KOしたときの音が流れたかどうか
	bool m_isPlayKOSE = false;

public:
	GameManager();
	virtual ~GameManager();
	void Init(const std::unique_ptr<Player>& _p1, const std::unique_ptr<Player>& _p2);
	void Update(GameData& _data);
	void Draw();
	void CheckFinish();
	int GetWinPlayer() const {return m_winPlayer;};
	bool GetIsFinish() const { return m_isFinish; };
	bool GetGoNextPhase() const { return m_nextPhase; };
	void SetNextPhase(bool _nextPhase);
	bool GetIsStart() const { return m_isStart; };
	bool GetIs2Point() const { return m_is2Point; };

private:
	void PlayStartSE();
	void PlayKOSE();
};