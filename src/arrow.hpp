#pragma once

#include "gameobject.hpp"
#include "body.hpp"
#include "tickable.hpp"
#include "drawable.hpp"
#include "counter.hpp"

class Arrow : public GameObject,
              public Body,
              public Tickable,
              public Drawable {
public:
	Arrow();
	~Arrow();
	
	void onInit();
	void onKill();
	void onTick(float dt);
	void onDraw();
	
	void setDirection(const Vec2& dir);
	void setSpeed(const float speed);
	
	Vec2  getDirection() const;
	float getSpeed() const;
	float getLength() const;
	float getAlpha() const;
	
private:
	bool    mStuck;
	float   mSpeed;
	Counter mFadeTimer;
};
