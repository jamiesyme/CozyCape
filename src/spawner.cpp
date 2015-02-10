#include "spawner.hpp"
#include "enemy.hpp"
#include "entitymanager.hpp"
#include <cstdlib>

Spawner::Spawner()
{
	mSpawnTimer.set(0.0);
}

Spawner::~Spawner()
{
}

void Spawner::onTick()
{
	if (mSpawnTimer.get() > 3.0) 
	{
		if (mPoints.size() > 0) 
		{
			const int index  = rand() % (int)mPoints.size();
			const Vec2 point = mPoints[index];
			//mPoints.erase(mPoints.begin() + index);
			
			Enemy* e = new Enemy();
			e->setPosition(point);
			getManager()->manage(e);
		}
		mSpawnTimer.set(0.0);
	}
}

void Spawner::addPoint(const Vec2& p)
{
	mPoints.push_back(p);
}
