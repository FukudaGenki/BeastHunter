#pragma once
#include "Vec2.h"
class TimeUIManager
{
private:
	int m_numberHandle = 0;

	// 表示する数を入れる変数（0に一桁目、1に二桁目）
	int m_currentCount[2] = {};
	// 画像全体の大きさ
	int m_sizeX = 0;
	int m_sizeY = 0;
	
public:
	TimeUIManager();
	virtual ~TimeUIManager();
	void Init();
	void Update(int _frame);
	void Draw();
};