#pragma once
class PointUI
{
private:
	int m_point = 0;
	int m_starHandle = 0;
	int m_starBackHandle = 0;

public:
	PointUI();
	~PointUI();
	void Init();
	void Update(int _point);
	void Draw(int _posX, int _posY);
};