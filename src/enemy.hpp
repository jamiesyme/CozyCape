#pragma once

#include "entity.hpp"
#include "clock.hpp"
class SimplePath;

class Enemy : public Entity {
public:
	Enemy();
	~Enemy();
	
	void onTick();
	void onDraw();
	
	float getRange() const;

private:
	Clock::Timer mScanTimer;
	Clock::Timer mPathTimer;
	Clock::Timer mTickTimer;
	SimplePath* mPathHead;
	SimplePath* mPathNode;
	bool mIsChasing;
};
