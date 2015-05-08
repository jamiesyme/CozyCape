#pragma once

#include "weapon.hpp"
#include "gameobject.hpp"
#include "tickable.hpp"
#include "drawable.hpp"
#include "counter.hpp"
#include "body.hpp"

class Sword : public Weapon,
              public GameObject,
              public Tickable,
              public Drawable,
              public Body {
public:
	Sword(Body* user);
	~Sword();
	
	void onInit();
	void onKill();
	void onTick(float dt);
	void onDraw();
	
	void ready();
	void attack();
	
	float getLength() const;
	Vec2  getDirection() const;

private:
	Body*   mUser;
	Counter mTimer;
	float   mBaseRot;
	float   mCurRot;
};
