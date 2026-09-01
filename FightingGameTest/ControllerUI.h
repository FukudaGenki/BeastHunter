#pragma once


struct GraphData
{
	int handle;
	int sizeX;
	int sizeY;
};

class ControllerUI
{
private:
	GraphData m_buttonAData = {};
	GraphData m_buttonAPressData = {};
	GraphData m_rightArrowData = {};
	bool m_isDecide = false;

public:
	ControllerUI();
	~ControllerUI();
	void Init();
	void Update();
	void Draw(float _posX, float _posY);
	void SetIsDecide(bool _decide) { m_isDecide = _decide; };

private:
	// 画像サイズ取得
	void GetGraficSizeFromData(GraphData& _data);
	// 中心描画
	void DrawCenter(GraphData _data, float _posX, float _posY, bool _isTurn);
};