#pragma once
#include <memory>
#include "Player.h"
class BulletManager
{
private:

public:
	BulletManager();
	virtual ~BulletManager();
	void Init(const std::unique_ptr<Player>& _p1, const std::unique_ptr<Player>& _p2);
	void Update();
	void Draw();
};