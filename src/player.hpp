#pragma once

#include "gameobject.hpp"
#include "body.hpp"
#include "tickable.hpp"
#include "drawable.hpp"
#include "trigger.hpp"
class Bow;
class Sword;
class Weapon;

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
	void onMessage(const std::string& msg, GameObject* go);
	void onMouseMove(const int x, const int y);
	void onMouseDown(const std::string& button);
	void onMouseUp(const std::string& button);
	
	float getSpeed() const;

	void  damage(const float dmg);
	float getHealth() const;

private:
	Bow*    mBow;
	Sword*  mSword;
	Weapon* mWeapon;
	float mHealth;
};
