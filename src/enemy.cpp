#include "enemy.hpp"
#include "simplepath.hpp"
#include "map.hpp"
#include "game.hpp"
#include "player.hpp"
#include "commongl.hpp"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

Enemy::Enemy()
{
	mScanTimer.set(0.0);
	mPathTimer.set(0.0);
	mTickTimer.set(0.0);
	mPathHead = new SimplePath();
	mPathNode = 0;
	mIsChasing = false;
	setRadius(0.4f);
}

Enemy::~Enemy()
{
	delete mPathHead;
}

void Enemy::onTick()
{
	const float kSpeed = 5.0f;
	
	// Check for line of sight
	if (mScanTimer.get() > 0.2) {
		mIsChasing = false;
		const Vec2 toPlayer = (Game::getPlayer()->getPos() - getPos());
		
		if (toPlayer.length() <= getRange()) {
			Map::Raycast ray;
			ray.pos = getPos();
			ray.dir = toPlayer.normalized();
			ray.dist = MIN(toPlayer.length(), getRange());
			if (!Map::tileRaycast(ray, 0))
				mIsChasing = true;
		}	
		mScanTimer.set(0.0);
	}
	
	// Get delta time
	const float dt = (float)mTickTimer.get();
	mTickTimer.set(0.0);
	
	// The player is in line of sight
	if (mIsChasing) 
	{
		Vec2 dir = (Game::getPlayer()->getPos() - getPos()).normalized();
		move(dir * dt * kSpeed);
	} else 
	// Do some pathfinding to find the player
	{
		if (mPathTimer.get() > 1.0) {
			mPathHead->reset();
			if (Map::pathfind(getPos(), Game::getPlayer()->getPos(), mPathHead))
				mPathNode = mPathHead;
			else
				mPathNode = 0;
			mPathTimer.set(0.0);
		}
		
		if (mPathNode != 0) {
			Vec2  diff = (mPathNode->getPos() - getPos());
			float dist = MIN(kSpeed * dt, diff.length());
			move(diff.normalized() * dist);
		
			if ((mPathNode->getPos() - getPos()).length() <= getRadius())
				mPathNode = mPathNode->getNext();
		}
	}
}

void Enemy::onDraw()
{
	if (mIsChasing)
		CommonGL::setColor(Color::Red);
	else
		CommonGL::setColor(Color::Grey);
	CommonGL::drawCircle(getPos(), getRadius(), 24);
	CommonGL::setColor(Color::White);
	CommonGL::drawCircle(getPos(), getRadius() * 0.8f, 24);
}

float Enemy::getRange() const
{
	return 5.0f;
}
