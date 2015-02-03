#pragma once

#include "entity.hpp"
#include "eventear.hpp"
#include "clock.hpp"

class Player : public Entity, public EventEar {
public:
	Player();
	~Player();
	
	void onTick();
	void onMouseMove(const int x, const int y);
	void onMouseDown(const std::string& button);
	void onMouseUp(const std::string& button);
	
	void  pullShot();
	void  releaseShot();
	bool  isShotHeld() const;
	float getShotPower() const;

private:
	Clock::Timer mTickTimer;
	Clock::Timer mShotTimer;
	bool mIsShotHeld;
};
