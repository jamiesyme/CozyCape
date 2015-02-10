#pragma once

#include "entity.hpp"
#include "clock.hpp"

class Arrow : public Entity {
public:
	Arrow();
	~Arrow();
	
	void onTick();
	void onDraw();
	
	void setDirection(const Vec2& dir);
	void setSpeed(const float speed);
	
	Vec2  getDirection() const;
	float getSpeed() const;
	float getLength() const;
	float getAlpha() const;
	
private:
	Clock::Timer mTickTimer;
	float  mSpeed;
	double mHitTime;
};
