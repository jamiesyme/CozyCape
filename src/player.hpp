#pragma once

#include "entity.hpp"
#include "eventear.hpp"
#include "clock.hpp"

class Player : public Entity, public EventEar {
public:
	Player();
	~Player();
	
	void onTick();
	void onDraw();
	void onMessage(const std::string& s, void* d);
	void onMouseMove(const int x, const int y);
	void onMouseDown(const std::string& button);
	void onMouseUp(const std::string& button);
	
	void     pullArrow();
	void  releaseArrow();
	bool   isArrowHeld()  const;
	float getArrowPower() const;

	void damage(const float dmg);

private:
	Clock::Timer mTickTimer;
	Clock::Timer mShotTimer;
	bool mIsArrowHeld;
	float mHealth;
};
