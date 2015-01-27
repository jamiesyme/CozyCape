#pragma once

#include "entity.hpp"
#include "clock.hpp"

class Player : public Entity {
public:
	Player();
	~Player();
	
	void onTick();

private:
	Clock::Timer mTickTimer;
	Clock::Timer mShotTimer;
};
