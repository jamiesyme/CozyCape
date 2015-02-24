#pragma once

#include "gameobject.hpp"
#include "body.hpp"
#include "tickable.hpp"
#include "drawable.hpp"
#include "trigger.hpp"

class Player : public GameObject,
               public Body,
               public Tickable,
               public Drawable,
               public Trigger {
public:
	Player();
	~Player();
	
	void onInit();
	void onKill();
	void onTick(float dt);
	void onDraw();
	void onMessage(const std::string& s, void* d);
	void onMouseMove(const int x, const int y);
	void onMouseDown(const std::string& button);
	void onMouseUp(const std::string& button);
	
	void    pullArrow();
	void releaseArrow();
	bool isArrowHeld() const;
	
	float getSpeed() const;

	void  damage(const float dmg);
	float getHealth() const;

private:
	bool  mIsArrowHeld;
	float mHealth;
};
