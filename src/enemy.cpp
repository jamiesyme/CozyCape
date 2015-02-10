#include "enemy.hpp"
#include "entitymanager.hpp"
#include "player.hpp"
#include "pathfinder.hpp"
#include "raycast.hpp"
#include "commongl.hpp"
#include "arrow.hpp"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

Enemy::Enemy()
{
	mPlayer = 0;
	mScanTimer.set(0.0);
	mPathTimer.set(0.0);
	mTickTimer.set(0.0);
	mHitTimer.set(0.0);
	mPathFinder = 0;
	mPathIndex = -1;
	mIsChasing = false;
	mHealth = 100.0f;
	mForceAffect = -1.0f;
	setType("enemy");
	setBodyCircle(0.4f);
}

Enemy::~Enemy()
{
}

void Enemy::onTick()
{
	// Kill us
	if (mHealth <= 0.0f) {
		getManager()->remove(this);
		return;
	}

	// Get the Player and PathFinder
	if (mPlayer == 0) {
		mPlayer = getManager()->findByType("player");
		if (mPlayer == 0)
			return;
	}
	if (mPathFinder == 0) {
		mPathFinder = (PathFinder*)getManager()->findByType("pathfinder");
		if (mPathFinder == 0)
			return;
	}
	
	// Get the delta time
	const float dt = (float)mTickTimer.get();
	mTickTimer.set(0.0);
	
	// Add the pusing force
	if (mForceAffect > 0.0f) {
		translate(mForce * 7.0f * mForceAffect * dt);
		mForceAffect -= dt * 4;
		return;
	}
	
	// Check for line of sight
	if (mScanTimer.get() > 0.2) 
	{
		mIsChasing = false;
		const Vec2 toPlayer = (mPlayer->getPosition() - getPosition());
		
		if (toPlayer.length() <= getRange()) 
		{
			Ray ray(getPosition(), toPlayer.normalized());
			RaycastInfo info;
			if (getManager()->doRaycast(ray, &info) && 
					info.entity == mPlayer)
				mIsChasing = true;
		}
		mScanTimer.set(0.0);
	}
	
	// Chase the player if they're in sight
	if (mIsChasing) 
	{
		moveTowards(mPlayer->getPosition(), dt);
	}
	
	// Otherwise, find a path to the player
	else 
	{
		// Find path
		if (mPathTimer.get() > 1.0) {
			mPath.clear();
			mPathIndex = -1;
			const Vec2 start = getPosition();
			const Vec2 end   = mPlayer->getPosition();
			if (mPathFinder->findPath(start, end, &mPath))
				mPathIndex = 0;
			mPathTimer.set(0.0);
		}
		
		// Follow path
		if (mPathIndex >= 0 && mPathIndex < mPath.getCount()) 
		{
			const Vec2 p = mPath.get(mPathIndex);
			moveTowards(p, dt);
			if ((p - getPosition()).length() <= getBodyRadius())
				mPathIndex += 1;
		}
	}
	
	// Hit the player
	if ((mPlayer->getPosition() - getPosition()).length() < 1.0f &&
	    mHitTimer.get() > 1.0)
	{
		mPlayer->onMessage("hit by enemy", this);
		mHitTimer.set(0.0);
	}
}

void Enemy::onDraw()
{
	if (mIsChasing)
		CommonGL::setColor(Color::Red);
	else
		CommonGL::setColor(Color::Grey);
	CommonGL::drawCircle(getPosition(), getBodyRadius(), 24);
	CommonGL::setColor(Color::White);
	CommonGL::drawCircle(getPosition(), getBodyRadius() * 0.8f, 24);
}

void Enemy::onMessage(const std::string& s, void* d)
{
	if (s == "hit by arrow") {
		Arrow* arrow = (Arrow*)d;
		Vec2 arrowDir = arrow->getDirection();
		Vec2 eToArrow = (getPosition() - arrow->getPosition()).normalized();
		float dot = eToArrow.dot(arrowDir);
		//mForce = eToArrow;
		mForce = arrowDir;
		mForceAffect = dot;
		
		damage(25.0f);
	}
}

void Enemy::moveTowards(const Vec2& p, const float dt)
{
	const Vec2 diff = p - getPosition();
	const float mag = MIN(diff.length(), dt * getSpeed());
	translate(diff.normalized() * mag);
}

float Enemy::getRange() const
{
	return 5.0f;
}

float Enemy::getSpeed() const
{
	return 4.5f;
}

void Enemy::damage(const float dmg)
{
	mHealth -= dmg;
}
