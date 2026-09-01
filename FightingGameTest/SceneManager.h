#pragma once
#include <memory>
#include "SceneBase.h"
#include "SceneMain.h"
#include "ResultScene.h"
#include "SceneID.h"
#include "GameData.h"
#include "Pad.h"
#include "TitleScene.h"
#include "SelectScene.h"

class SceneManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneManager();
	/// <summary>
	/// 初期化
	/// </summary>
	static void Init();
	/// <summary>
	/// シーンを引数によって変える関数
	/// </summary>
	/// <param name="scene">変更するシーン</param>
	static void ChangeScene(SceneID scene);
	/// <summary>
	/// 現在のシーンの更新処理
	/// </summary>
	static void Update(const Pad& _pad, const Pad& _pad2);
	/// <summary>
	/// 現在のシーンの描画処理
	/// </summary>
	static void Render();
public:
	static GameData m_data; //ゲームのデータを割り当てる構造体
private:
	static std::unique_ptr<SceneBase> m_Scene; // 現在のシーン
};