#pragma once

#include "entity.hpp"
#include "clock.hpp"
#include <vector>

class Arrow : public Entity {
public:
	Arrow();
	~Arrow();
	
	void onTick();
	void onDraw();
	
	void setDirection(const Vec2& dir);
	void setDuration(const float d);
	void shoot();
	
private:
	std::vector<Vec2> mTrail;
	Clock::Timer mTickTimer;
	Vec2  mDirection;
	float mDuration;
	float mAlpha;
};
