#include "enemy.hpp"
#include "player.hpp"
#include "pathfinder.hpp"
#include "commongl.hpp"
#include "arrow.hpp"
#include "sword.hpp"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

Enemy::Enemy()
{
	mScanTimer.setGoal(0.2f);
	mPathTimer.setGoal(1.0f);
	mAttackTimer.setGoal(0.75f);
	mAttackTimer.forceToGoal();
	mPlayer     = 0;
	mPathFinder = 0;
	mPathIndex  = -1;
	mIsPlayerInSight = false;
	mSpeed       = 4.5f;
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
	broadcastMessage("enemy died");
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
	
	// If we just attacked, we have to pause
	if (!mAttackTimer.reachedGoal())
		return;
	
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
	CommonGL::setColor(Color::Grey);
	if (mIsPlayerInSight) 
		CommonGL::setColor(Color::Red);
	if (!mAttackTimer.reachedGoal())
		CommonGL::setColor(Color(0.3f, 0.3f, 0.9f));           
	CommonGL::drawCircle(getPosition(), getBodyRadius(), 24);
	CommonGL::setColor(Color::White);
	CommonGL::drawCircle(getPosition(), getBodyRadius() * 0.8f, 24);
}

void Enemy::onMessage(const std::string& msg, GameObject* go)
{
	if (msg == "hit by arrow") {
		Arrow* arrow  = (Arrow*)go;
		Vec2 eToArrow = (getPosition() - arrow->getPosition()).normalized();
		float dot     = eToArrow.dot(arrow->getDirection());
		mForce        = arrow->getDirection();
		mForceAffect  = dot;
		
		damage(1.0f);
	} else
	
	if (msg == "hit by sword") {
		Sword* sword  = (Sword*)go;
		Vec2 eToArrow = (getPosition() - sword->getPosition()).normalized();
		float dot     = eToArrow.dot(sword->getDirection());
		mForce        = sword->getDirection();
		mForceAffect  = dot;
		
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

void Enemy::setSpeed(const float speed)
{
	mSpeed = speed;
}

float Enemy::getSpeed() const
{
	return mSpeed;
}

void Enemy::damage(const float dmg)
{
	mHealth -= dmg;
}
