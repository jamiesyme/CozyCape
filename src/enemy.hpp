#pragma once

#include "gameobject.hpp"
#include "body.hpp"
#include "tickable.hpp"
#include "drawable.hpp"
#include "counter.hpp"
#include "pathinfo.hpp"
class PathFinder;
class Player;

class Enemy : public GameObject,
              public Body,
              public Tickable,
              public Drawable {
public:
	Enemy();
	~Enemy();
	
	void onInit();
	void onKill();
	void onTick(float dt);
	void onDraw();
	void onMessage(const std::string& msg, GameObject* go);
	
	void moveTowards(const Vec2& p, const float dt);
	float getSightRange() const;
	float getAttackRange() const;
	
	void  setSpeed(const float speed);
	float getSpeed() const;
	
	void damage(const float dmg);

private:
	Counter mScanTimer;
	Counter mPathTimer;
	Counter mAttackTimer;
	Player*     mPlayer;
	PathFinder* mPathFinder;
	PathInfo    mPath;
	int         mPathIndex;
	bool  mIsPlayerInSight;
	float mSpeed;
	float mHealth;
	
	Vec2  mForce;
	float mForceAffect;
};
