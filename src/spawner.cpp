#include "spawner.hpp"
#include "bullet.hpp"
#include "map.hpp"
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
	if (mSpawnTimer.get() > 1.0) {
		if (mPoints.size() > 0) {
			const Vec2 point = mPoints[rand() % (int)mPoints.size()];
			Bullet* b[4];
			for (int i = 0; i < 4; i++) {
				b[i] = new Bullet();
				b[i]->setPos(point);
				b[i]->setDuration(0.5f);
				b[i]->setBounceMax(0);
				Map::manageEntity(b[i]);
			}
			b[0]->setDirection(Vec2(1.0f, 0.0f));
			b[1]->setDirection(Vec2(-1.0f, 0.0f));
			b[2]->setDirection(Vec2(0.0f, 1.0f));
			b[3]->setDirection(Vec2(0.0f, -1.0f));
			for (int i = 0; i < 4; i++)
				b[i]->fire();
		}
		mSpawnTimer.set(0.0);
	}
}

void Spawner::addPoint(const Vec2& p)
{
	mPoints.push_back(p);
}
