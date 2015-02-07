#include "enemy.hpp"
#include "game.hpp"
#include "player.hpp"
#include "pathfinder.hpp"
#include "raycast.hpp"
#include "commongl.hpp"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

Enemy::Enemy()
{
	mPlayer = 0;
	mScanTimer.set(0.0);
	mPathTimer.set(0.0);
	mTickTimer.set(0.0);
	mPathFinder = 0;
	mPathIndex = -1;
	mIsChasing = false;
	setType("enemy");
	setBodyCircle(0.4f);
}

Enemy::~Enemy()
{
}

void Enemy::onTick()
{
	// Get the Player and PathFinder
	if (mPlayer == 0) {
		mPlayer = Game::findEntityByType("player");
		if (mPlayer == 0)
			return;
	}
	if (mPathFinder == 0) {
		mPathFinder = (PathFinder*)Game::findEntityByType("pathfinder");
		if (mPathFinder == 0)
			return;
	}
	
	// Get the delta time
	const float dt = (float)mTickTimer.get();
	mTickTimer.set(0.0);
	
	// Check for line of sight
	if (mScanTimer.get() > 0.2) 
	{
		mIsChasing = false;
		const Vec2 toPlayer = (mPlayer->getPosition() - getPosition());
		
		if (toPlayer.length() <= getRange()) 
		{
			Ray ray(getPosition(), toPlayer.normalized());
			RaycastInfo info;
			if (Game::doRaycast(ray, &info) && info.entity == mPlayer)
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
