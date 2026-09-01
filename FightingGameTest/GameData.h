
#pragma once
enum class Chara
{
	Nova,
	Shartel,
	Max // 最大数
};

struct GameData
{
	int p1Point;
	int p2Point;
	// ラウンド開始からののフレーム数
	int roundFrame;
	// 選んだキャラクター
	Chara p1Chara;
	Chara p2Chara;
};