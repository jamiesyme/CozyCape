#include "spawner.hpp"
#include "enemy.hpp"
#include "gameobjectgod.hpp"
#include "tickgod.hpp"
#include <cstdlib>

Spawner::Spawner()
{
	mSpawnTimer.setGoal(2.75f);
	setType("spawner");
}

Spawner::~Spawner()
{
}

void Spawner::onInit()
{
	getTickGod()->addTo("spawners", this);
}

void Spawner::onKill()
{
	getTickGod()->removeFrom("spawners", this);
}

void Spawner::onTick(float dt)
{
	// Update the timer
	mSpawnTimer.addTime(dt);
	if (!mSpawnTimer.reachedGoal())
		return;
		
	// Reset the timer
	mSpawnTimer.reset();
		
	// Pick a random spawn point
	if (mPoints.size() <= 0)
		return;
	int  index = rand() % (int)mPoints.size();
	Vec2 point = mPoints[index];
	
	// Spawn the enemy
	Enemy* enemy = new Enemy();
	enemy->setPosition(point);
	getGod()->manage(enemy);
}

void Spawner::addPoint(const Vec2& p)
{
	mPoints.push_back(p);
}
