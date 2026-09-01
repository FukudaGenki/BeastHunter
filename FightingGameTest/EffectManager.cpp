#include "DxLib.h"
#include "EffectManager.h"

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::Init()
{
	// 情報の設定
	m_effect[static_cast<int>(EffectType::Hit)] = {"hitEffect/red.png",0, 2, 8, false};
	m_effect[static_cast<int>(EffectType::Guard)] = { "hitEffect/blue1.png", 0, 2, 11, false };
	
	// filePathからhandleへの変換
	for (int i = 0; i < static_cast<int>(EffectType::Max); i++)
	{
		m_effect[static_cast<int>(i)].handle = LoadGraph(m_effect[static_cast<int>(i)].filePath);
	}
	
	// アニメーションの初期化
	for (int i = 0; i < static_cast<int>(EffectType::Max); i++)
	{
		EffectData data = m_effect[i];
		m_p1Effect[i].Init(data.handle, data.animFrame, data.animNum, data.isLoop);
		m_p2Effect[i].Init(data.handle, data.animFrame, data.animNum, data.isLoop);
	}
}

void EffectManager::Update()
{
	// エフェクトの更新処理
	for (int i = 0; i < static_cast<int>(EffectType::Max); i++)
	{
		m_p1Effect[i].Update();
		m_p2Effect[i].Update();
	}
}

void EffectManager::Draw()
{
	// エフェクトの描画
	for (int i = 0; i < static_cast<int>(EffectType::Max); i++)
	{
		if (!m_p1Effect[i].IsEnd())
		{
			m_p1Effect[i].DrawCenter(m_p1Pos[i].x, m_p1Pos[i].y, false);
		}
		if (!m_p2Effect[i].IsEnd())
		{
			m_p2Effect[i].DrawCenter(m_p2Pos[i].x, m_p2Pos[i].y, false);
		}
	}
}

void EffectManager::SetPlay(int _playerIdx, EffectType _effectType, Vec2 _pos)
{
	int type = static_cast<int>(_effectType);
	if (_playerIdx == 1)
	{
		// エフェクトの生成位置
		m_p1Pos[type] = _pos;
		// エフェクトのアニメーションをリセット
		m_p1Effect[type].Reset();
	}
	else if (_playerIdx == 2)
	{
		// エフェクトの生成位置
		m_p2Pos[type] = _pos;
		// エフェクトアニメーションをリセット
		m_p2Effect[type].Reset();
	}
}
