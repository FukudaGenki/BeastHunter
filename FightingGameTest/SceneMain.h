#pragma once
#include <array>
#include <memory>
#include <vector>
#include "SceneBase.h"
#include "Player.h"
#include "Nova.h"
#include "Shartel.h"
#include "HadouBullet.h"
#include "Bg.h"
#include "ColCheck.h"
#include "UIManager.h"
#include "EffectManager.h"
#include "GameManager.h"


class SceneMain :public SceneBase
{
private:
	// ここでメモリの確保を行う
	std::array<std::unique_ptr<Player>, 2> m_Player = { nullptr, nullptr };
	std::array<std::unique_ptr<HadouBullet>, 2> m_Hadou = { nullptr, nullptr };
	std::unique_ptr<Bg> m_Bg = nullptr;
	std::unique_ptr<UIManager> m_UI = nullptr;
	std::unique_ptr<ColCheck> m_ColCheck = nullptr;
	std::unique_ptr<EffectManager> m_Effect = nullptr;
	std::unique_ptr<GameManager> m_GameManager = nullptr;
	// デバック用
	int m_isCol = 0;
	bool m_checkBool =true;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneMain();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneMain();
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
private:
	/// <summary>
	/// プレイヤーに関する処理
	/// </summary>
	void PlayerUpdate(const Pad& _pad, const Pad& _pad2);
	/// <summary>
	/// 弾を生み出すための関数
	/// </summary>
	void GeneBullet();
	/// <summary>
	/// 弾とプレイヤーの当たり判定をやる
	/// </summary>
	/// <param name="_pNum">弾を撃ったプレイヤー</param>
	/// <param name="_pColType">プレイヤーの当たり判定タイプ</param>
	/// <returns>当たったかどうか</returns>
	bool IsColBullet(int _pNum, int _enePNum, ColType _colType);
	/// <summary>
	/// エフェクトの生成を行うかどうかのチェック
	/// </summary>
	void CheckEffectGene();

	float GetScrollX(int _dir);

	void GoNextPhase();
};