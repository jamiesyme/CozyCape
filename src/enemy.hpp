#pragma once

#include "entity.hpp"
#include "clock.hpp"
#include "pathinfo.hpp"
class PathFinder;

class Enemy : public Entity {
public:
	Enemy();
	~Enemy();
	
	void onTick();
	void onDraw();
	void onMessage(const std::string& s, void* d);
	
	void moveTowards(const Vec2& p, const float dt);
	float getRange() const;
	float getSpeed() const;
	
	void damage(const float dmg);

private:
	Entity* mPlayer;
	Clock::Timer mScanTimer;
	Clock::Timer mPathTimer;
	Clock::Timer mTickTimer;
	Clock::Timer mHitTimer;
	PathFinder* mPathFinder;
	PathInfo mPath;
	int  mPathIndex;
	bool mIsChasing;
	float mHealth;
	
	Vec2  mForce;
	float mForceAffect;
};
