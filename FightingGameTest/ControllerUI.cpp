#include "ControllerUI.h"
#include "DxLib.h"
namespace
{
	constexpr float kDisButtonToArrow = 120.0f;
}

ControllerUI::ControllerUI()
{}

ControllerUI::~ControllerUI()
{
	DeleteGraph(m_buttonAData.handle);
	DeleteGraph(m_rightArrowData.handle);
	DeleteGraph(m_buttonAPressData.handle);
}

void ControllerUI::Init()
{
	m_buttonAData.handle = LoadGraph("UI/controller/buttonA.png");
	GetGraficSizeFromData(m_buttonAData);

	m_buttonAPressData.handle = LoadGraph("UI/controller/buttonAPress.png");
	GetGraficSizeFromData(m_buttonAPressData);

	m_rightArrowData.handle = LoadGraph("UI/controller/rightArrow.png");
	GetGraficSizeFromData(m_rightArrowData);

	// 変数の初期化
	m_isDecide = false;
}

void ControllerUI::Update()
{}

void ControllerUI::Draw(float _posX, float _posY)
{
	float x = _posX + 75;

	// Aボタン
	if (m_isDecide)
	{
		DrawCenter(m_buttonAPressData, x, _posY, false);
	}
	else
	{
		DrawCenter(m_buttonAData, x, _posY, false);
	}
	// 右矢印
	DrawCenter(m_rightArrowData, x + kDisButtonToArrow, _posY, false);
	// 左矢印
	DrawCenter(m_rightArrowData, x - kDisButtonToArrow, _posY, true);
}

void ControllerUI::DrawCenter(GraphData _data, float _posX, float _posY, bool _isTurn)
{
	// 画像サイズが0ならリターンする
	if (_data.sizeX <= 0 || _data.sizeY <= 0) return;

	int x = static_cast<int>(_posX);
	int y = static_cast<int>(_posY);
	int centerX = x - _data.sizeX / 2;
	int centerY = y - _data.sizeY / 2;

	DrawRotaGraph(centerX, centerY,1, 0, _data.handle, true, _isTurn, false);
}



void ControllerUI::GetGraficSizeFromData(GraphData& _data)
{
	GetGraphSize(_data.handle, &_data.sizeX, &_data.sizeY);
}
