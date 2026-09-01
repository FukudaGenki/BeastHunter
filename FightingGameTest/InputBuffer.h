#pragma once
#include <deque>
#include <string>
#include "Pad.h"

// 入力履歴とフレーム数を保存
struct InputLog
{
	int leverDirection; // 1～9のテンキー方向
	int frameCount; // 同じ入力が長押しされたフレーム数
};

class InputBuffer
{
private:
	InputLog m_current = { 0, false };
	std::deque <InputLog> m_history;
	int m_inputSameKeyFrame[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int m_playerIdx = 0;
	int m_fontHandle = 0;

public:
	InputBuffer();
	virtual ~InputBuffer();
	void Init(int _pIdx);
	void Update(const Pad& _pad);
	void Draw();
	const std::deque<InputLog>& GetHistory() const { return m_history; }
	/// <summary>
	/// テンキー方向を矢印方向に変換
	/// </summary>
	/// <returns>矢印</returns>
	std::string TenKeyToAllow(int _tenKey);
};