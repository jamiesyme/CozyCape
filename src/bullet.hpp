#pragma once

#include "entity.hpp"
#include "clock.hpp"
#include <vector>

class Bullet : public Entity {
public:
	Bullet();
	~Bullet();
	
	void onTick();
	void onDraw();
	
	void setDirection(const Vec2& dir);
	void setBounceMax(const int i);
	void setDuration(const float d);
	void fire();
	
private:
	std::vector<Vec2> mTrail;
	Clock::Timer mTickTimer;
	Vec2  mDirection;
	int   mBounceMax;
	float mDuration;
	float mAlpha;
};
