#pragma once
#include "GameData.h"
#include "Animation.h"
#include <map>

enum class SelectAnimID
{
	NovaIdle,
	ShartelIdle
};

struct SelectAnimData
{
	const char* filePath; // 画像のパス
	int animFrame; // 1コマの表示フレーム数
	int animNum; // アニメーションのコマ数
	bool isLoop; // ループするかどうか
};

// ファイルパスと画像サイズを構造体
struct GraficData
{
	int handle;
	int sizeX;
	int sizeY;
};

// セレクトシーンで描画するキャラ（１P、２Pがセレクトしているキャラを描画する）
class SelectChara
{
private:
	// 現在選択しているキャラクター
	Chara m_selectChara = Chara::Max;

	// アニメーションのマップ（それぞれのキャラクターのアイドル状態のアニメーション）
	std::map<SelectAnimID, Animation> m_animations;
	// 現在再生中のアニメーションのポインタ
	Animation* m_pCurrentAnim = nullptr;
	// 現在のアニメーションID
	SelectAnimID m_currentAnimID = SelectAnimID::NovaIdle;
	// 前フレームのアニメーション（アニメーションを変更を知るため）
	SelectAnimID m_lastAnimID = SelectAnimID::NovaIdle;

	// 文字の画像ハンド折る
	// キャラクター
	GraficData m_novaCharaName = {};
	GraficData m_shartelCharaName = {};
	// VS
	GraficData m_VSFont = {};

	

public:
	SelectChara();
	~SelectChara();
	void Init();
	void Update(Chara _selectChara);
	void Draw(float _posX, float _posY, bool _isTurn);
private:
	// 選択キャラクターを変更する
	void CheckChangeAnim();
	// 表示文字の変更
	void ChangeCharaName();
	// 中心描画
	void DrawCenter(GraficData _data, float _posX, float _posY, bool _isTurn);
	// 画像サイズ取得
	void GetGraficSizeFromData(GraficData& _data);
};