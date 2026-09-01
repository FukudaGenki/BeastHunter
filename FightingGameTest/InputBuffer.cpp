#include "DxLib.h"
#include "InputBuffer.h"
#include <DxLib.h>

namespace
{
	// 入力履歴を出す最大行数
	constexpr int kMaxLogLines = 20;

	// 入力履歴の表示に使う定数
	constexpr int kP1Pos = 50;
	constexpr int kP2Pos = 1500;
	constexpr int kBetweenLines = 25;
	constexpr int kPosY = 150;
}

InputBuffer::InputBuffer()
{
	
}

InputBuffer::~InputBuffer()
{
	DeleteFontToHandle(m_fontHandle);
}

void InputBuffer::Init(int _pIdx)
{
	m_fontHandle = CreateFontToHandle(NULL, 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_playerIdx = _pIdx;
}



void InputBuffer::Update(const Pad& _pad)
{
	// 入力の取得
	m_current.leverDirection = _pad.ConvertToTenKey();

	// 履歴がない、または入力変更された場合
	if (m_history.empty() || m_history.back().leverDirection != m_current.leverDirection)
	{
		m_current.frameCount = 1;
		m_history.push_back(m_current);

		// 保存する最大行数より大きくなったら削除
		if (m_history.size() > kMaxLogLines)
		{
			m_history.pop_front();
		}
	}
	// 入力が変更されない場合
	else
	{
		// 入力方向が変わってないならログに１足す
		if (m_history.back().frameCount < 99)
		{
			m_history.back().frameCount++;
		}
	}


#ifdef _DEBUG
	//clsDx();

	/*for (int i = 0; i < m_history.size(); i++)
	{
		printfDx("%s%d\n", TenKeyToAllow(m_history[i].leverDirection).c_str(), m_history[i].frameCount);
	}*/
#endif
}

void InputBuffer::Draw()
{
#ifdef _DEBUG
	if (m_playerIdx == 1)
	{
		for (int i = 0; i < m_history.size(); i++)
		{
			//DrawFormatString(kP1Pos, i * kBetweenLines + kPosY, 0xff0000, "%s%d\n", TenKeyToAllow(m_history[i].leverDirection).c_str(), m_history[i].frameCount);
			DrawFormatStringToHandle(
				kP1Pos, i * kBetweenLines + kPosY,
				0xff0000, m_fontHandle,
				"%2s%d\n", TenKeyToAllow(m_history[i].leverDirection).c_str(), m_history[i].frameCount
				);
		}
	}
	else if (m_playerIdx == 2)
	{
		for (int i = 0; i < m_history.size(); i++)
		{
			//DrawFormatString(kP2Pos, i * kBetweenLines + kPosY, 0xff0000, "%s%d\n", TenKeyToAllow(m_history[i].leverDirection).c_str(), m_history[i].frameCount);
			DrawFormatStringToHandle(
				kP2Pos, i * kBetweenLines + kPosY,
				0xff0000, m_fontHandle,
				"%2s%d\n", TenKeyToAllow(m_history[i].leverDirection).c_str(), m_history[i].frameCount
				);
		}
	}
#endif
}

std::string InputBuffer::TenKeyToAllow(int _tenKey)
{
	if (_tenKey == 5)
	{
		return "N";
	}
	else if (_tenKey == 1)
	{
		return "←↓";
	}
	else if (_tenKey == 2)
	{
		return "↓";
	}
	else if (_tenKey == 3)
	{
		return "→↓";
	}
	else if (_tenKey == 4)
	{
		return "←";
	}
	else if (_tenKey == 6)
	{
		return "→";
	}
	else if (_tenKey == 7)
	{
		return "←↑";
	}
	else if (_tenKey == 8)
	{
		return "↑";
	}
	else if (_tenKey == 9)
	{
		return "→↑";
	}
	else
	{
		return "?";
	}
}