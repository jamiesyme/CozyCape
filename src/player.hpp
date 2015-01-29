#pragma once

#include "entity.hpp"
#include "eventear.hpp"
#include "clock.hpp"

class Player : public Entity, public EventEar {
public:
	Player();
	~Player();
	
	void onTick();
	void onMouseDown(const std::string& button);
	
	void shoot();

private:
	Clock::Timer mTickTimer;
	Clock::Timer mShotTimer;
};
