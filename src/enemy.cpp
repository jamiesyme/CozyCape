#include "enemy.hpp"
#include "gameobjectgod.hpp"
#include "tickgod.hpp"
#include "drawgod.hpp"
#include "physicsgod.hpp"
#include "player.hpp"
#include "pathfinder.hpp"
#include "commongl.hpp"
#include "arrow.hpp"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

Enemy::Enemy()
{
	mScanTimer.setGoal(0.2f);
	mPathTimer.setGoal(1.0f);
	mAttackTimer.setGoal(0.75f);
	mPlayer     = 0;
	mPathFinder = 0;
	mPathIndex  = -1;
	mIsPlayerInSight = false;
	mHealth      = 4.0f;
	mForceAffect = -1.0f;
	setType("enemy");
	setBodyCircle(0.4f);
	setBodyGo(this);
}

Enemy::~Enemy()
{
}

void Enemy::onInit()
{
	getTickGod()->addTo("enemies", this);
	getDrawGod()->addTo("enemies", this);
	getPhysicsGod()->add(this);
}

void Enemy::onKill()
{
	getTickGod()->removeFrom("enemies", this);
	getDrawGod()->removeFrom("enemies", this);
	getPhysicsGod()->remove(this);
}

void Enemy::onTick(float dt)
{
	// Kill us
	if (mHealth <= 0.0f) {
		getGod()->remove(this);
		return;
	}

	// Get the Player and PathFinder
	if (mPlayer == 0) {
		mPlayer = (Player*)getGod()->findByType("player");
		if (mPlayer == 0)
			return;
	}
	if (mPathFinder == 0) {
		mPathFinder = (PathFinder*)getGod()->findByType("pathfinder");
		if (mPathFinder == 0)
			return;
	}
	
	// Update the timers
	mScanTimer.addTime(dt);
	mPathTimer.addTime(dt);
	mAttackTimer.addTime(dt);
	
	// Add the pushing force
	if (mForceAffect > 0.0f) {
		translate(mForce * 7.0f * mForceAffect * dt);
		mForceAffect -= dt * 4;
		return;
	}
	
	// Update line of sight flag
	if (mScanTimer.reachedGoal()) 
	{
		// Reset the timer
		mScanTimer.reset();
	
		// Construct ray
		Vec2  dirToPlayer = mPlayer->getPosition() - getPosition();
		float rayLength   = getSightRange();
		Raycast ray(getPosition(), dirToPlayer);
		
		// Update sight
		mIsPlayerInSight = false;
		if (getGod()->doRaycast(ray, rayLength)) {
			if (ray.hitBody->getBodyGo() == mPlayer)
				mIsPlayerInSight = true;
		}
	}
	
	// Calculate new path to player (if they're not in sight)
	if (!mIsPlayerInSight && mPathTimer.reachedGoal())
	{
		// Reset the timer
		mPathTimer.reset();
		
		// Reset the path
		mPath.clear();
		mPathIndex = -1;
		
		// Find new path
		Vec2 start = getPosition();
		Vec2 end   = mPlayer->getPosition();
		if (mPathFinder->findPath(start, end, &mPath))
			mPathIndex = 0;
	}
	
	// Move towards player
	if (mIsPlayerInSight) 
	{
		// Run directly at the player
		moveTowards(mPlayer->getPosition(), dt);
	} else
	if (mPathIndex >= 0 && 
	    mPathIndex < mPath.getCount()) 
	{
		// Run towards the next point
		Vec2 nextPoint = mPath.get(mPathIndex);
		moveTowards(nextPoint, dt);
		
		// Are we close enough to switch points
		if ((getPosition() - nextPoint).length() < getBodyRadius())
			mPathIndex++;
	}
	
	// Attack the player
	if (mAttackTimer.reachedGoal())
	{
		float distToPlayer = (mPlayer->getPosition() - getPosition()).length();
		distToPlayer -= mPlayer->getBodyRadius();
		if (distToPlayer < getAttackRange())
		{
			mAttackTimer.reset();
			mPlayer->onMessage("hit by enemy", this);
		}
	}
}

void Enemy::onDraw()
{
	if (mIsPlayerInSight) CommonGL::setColor(Color::Red);
	else                  CommonGL::setColor(Color::Grey);
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
		
		damage(1.0f);
	}
}

void Enemy::moveTowards(const Vec2& p, const float dt)
{
	const Vec2 diff = p - getPosition();
	const float mag = MIN(diff.length(), dt * getSpeed());
	translate(diff.normalized() * mag);
}

float Enemy::getSightRange() const
{
	return 5.0f;
}

float Enemy::getAttackRange() const
{
	return getBodyRadius() + 0.1f;
}

float Enemy::getSpeed() const
{
	return 4.5f;
}

void Enemy::damage(const float dmg)
{
	mHealth -= dmg;
}
