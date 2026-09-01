#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{
private:
	int m_titleHandle = 0;
	int m_titleSizeX = 0;
	int m_titleSizeY = 0;

	int m_pressAHandle = 0;
	bool m_isPressAActive = false;
	int m_frameTimer = 0;

	// 透明度
	int m_blendIdx = 0;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~TitleScene();
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
	void DrawCenter(int _handle, float _posX, float _posY, int _sizeX, int _sizeY);
};