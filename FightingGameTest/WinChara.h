#pragma once
#include "GameData.h"
#include "Animation.h"
#include <map>

enum class WinAnimID
{
	Nova,
	Shartel
};

struct WinAnimData
{
	const char* filePath; // 画像のパス
	int animFrame; // 1コマの表示フレーム数
	int animNum; // アニメーションのコマ数
	bool isLoop; // ループするかどうか
};

// ファイルパスと画像サイズを構造体
struct WinGraficData
{
	int handle;
	int sizeX;
	int sizeY;
};

// セレクトシーンで描画するキャラ（１P、２Pがセレクトしているキャラを描画する）
class WinChara
{
private:
	// 現在選択しているキャラクター
	Chara m_WinChara = Chara::Max;

	// アニメーションのマップ（それぞれのキャラクターのアイドル状態のアニメーション）
	std::map<WinAnimID, Animation> m_animations;
	// 現在再生中のアニメーションのポインタ
	Animation* m_pCurrentAnim = nullptr;

	// 文字の画像ハンド折る
	// キャラクター
	WinGraficData m_p1Name = {};
	WinGraficData m_p2Name = {};

public:
	WinChara();
	~WinChara();
	void Init(Chara _winChara);
	void Update();
	void Draw(float _posX, float _posY, bool _isTurn);
private:
	// 中心描画
	void DrawCenter(WinGraficData _data, float _posX, float _posY, bool _isTurn);
	// 画像サイズ取得
	void GetGraficSizeFromData(WinGraficData& _data);
};
