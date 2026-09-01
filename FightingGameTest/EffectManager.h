#pragma once
#include "Vec2.h"
#include "Animation.h"
#include <map>

enum class EffectType
{
	Hit,
	Guard,
	Max
};

struct EffectData
{
	const char* filePath; // 画像のパス
	int handle;
	int animFrame; // 1コマの表示フレーム数
	int animNum; // アニメーションのコマ数
	bool isLoop; // ループするかどうか
	bool active;
};

class EffectManager
{
private:
	// 現状１プレイヤーあたり一つのエフェクトしかできないようになってる
	// エフェクトの情報を入力する構造体
	EffectData m_effect[static_cast<int>(EffectType::Max)] = {};
	// アニメーションのクラス
	Animation m_p1Effect[static_cast<int>(EffectType::Max)] = {};
	Animation m_p2Effect[static_cast<int>(EffectType::Max)] = {};
	// アニメーションの位置
	Vec2 m_p1Pos[static_cast<int>(EffectType::Max)] = {};
	Vec2 m_p2Pos[static_cast<int>(EffectType::Max)] = {};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EffectManager();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// エフェクトを流す
	/// </summary>
	void SetPlay(int _playerIdx, EffectType _effectType, Vec2 _pos);
private:
	

};