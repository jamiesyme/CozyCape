#include "spawner.hpp"
#include "enemy.hpp"
#include "map.hpp"
#include <cstdlib>

Spawner::Spawner()
{
	mSpawnTimer.set(0.0);
	setSolid(false);
}

Spawner::~Spawner()
{
}

void Spawner::onTick()
{
	if (mSpawnTimer.get() > 1.0) {
		if (mPoints.size() > 0) {
			const int index  = rand() % (int)mPoints.size();
			const Vec2 point = mPoints[index];
			mPoints.erase(mPoints.begin() + index);
			
			Enemy* e = new Enemy();
			e->setPos(point);
			Map::manageEntity(e);
		}
		mSpawnTimer.set(0.0);
	}
}

void Spawner::onDraw()
{
	// Don't draw anything.
}

void Spawner::addPoint(const Vec2& p)
{
	mPoints.push_back(p);
}
