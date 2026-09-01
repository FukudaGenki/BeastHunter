#pragma once
#include "SceneBase.h"
#include "SelectChara.h"
#include "ControllerUI.h"



struct SelectCharaData
{
	Chara chara;
	bool isDecide;
};

class SelectScene : public SceneBase
{
private:
	// キャラクターが決定してからスタートするまで
	int m_startFrameTimer = 0;

	SelectCharaData m_p1Select = {Chara::Nova, false};
	SelectCharaData m_p2Select = {Chara::Nova, false};

	std::unique_ptr<SelectChara> m_p1Chara = nullptr;
	std::unique_ptr<SelectChara> m_p2Chara = nullptr;

	std::unique_ptr<ControllerUI> m_Controller = nullptr;
	std::unique_ptr<ControllerUI> m_Controller2 = nullptr;

	// 火のエフェクトで使う変数
	// フレームタイマー
	int m_frameTimer = 0;
	int m_fireEffectHandle = 0;
	Animation* m_pFireEffectAnim = nullptr;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectScene();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SelectScene();
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