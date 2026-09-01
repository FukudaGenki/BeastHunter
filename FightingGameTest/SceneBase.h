#pragma once
#include "SceneID.h"
#include "GameData.h"
#include "Pad.h"
#include <memory>
//#include "Background.h"

class SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneBase();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneBase();
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(GameData& data) = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(GameData& data, const Pad& _pad, const Pad& _pad2) = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(GameData& data) = 0;
protected:
	void ChangeScene(SceneID scene);

protected:
	//std::unique_ptr<Background> m_background;
};

