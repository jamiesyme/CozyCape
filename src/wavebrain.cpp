#include "wavebrain.hpp"
#include "vec2.hpp"
#include "enemy.hpp"
#include <iostream>

class WaveBrain::Spawner {
public:
	Vec2 mPoint;
	
	Spawner(Vec2 point)
	{
		mPoint = point;
	}
	
	// Just *creates* the entity
	Enemy* spawnEnemy() 
	{
		Enemy* enemy = new Enemy();
		enemy->setPosition(mPoint);
		return enemy;
	}
};

WaveBrain::WaveBrain()
{
	setWave(1);
	mSpawners.push_back( Spawner(Vec2(13.5f, 13.5f)) );
	mSpawners.push_back( Spawner(Vec2(27.5f, 11.5f)) );
	mSpawners.push_back( Spawner(Vec2( 4.5f, 12.5f)) );
	mSpawners.push_back( Spawner(Vec2(24.5f, 30.5f)) );
	setType("wave brain");
}

WaveBrain::~WaveBrain()
{
}

void WaveBrain::onInit()
{
	getTickGod()->addTo("spawners", this);
}

void WaveBrain::onKill()
{
	
	getTickGod()->removeFrom("spawners", this);
}

void WaveBrain::onTick(float dt)
{
	// Spawn homies
	mSpawnTimer.addTime(dt);
	if (mSpawnTimer.reachedGoal() && mEnemiesSpawned < mSpawnCount) 
	{
		mSpawnTimer.reset();
		Spawner& spawner = mSpawners[rand() % (int)mSpawners.size()];
		Enemy* enemy = spawner.spawnEnemy();
		enemy->setSpeed(mSpawnSpeed);
		enemy->addListener(this);
		getGod()->manage(enemy);
		mEnemiesSpawned++;
	}
}

void WaveBrain::onMessage(const std::string& msg, GameObject* go)
{
	if (msg == "enemy died") {
		mEnemiesLeft--;
		if (mEnemiesLeft <= 0)
			incrementWave();
	}
}

void WaveBrain::incrementWave()
{
	mWave           = mWave + 1;
	mSpawnSpeed     = mSpawnSpeed + 0.25f;
	mSpawnWait      = mSpawnWait  * 0.9f;
	mSpawnCount     = mSpawnCount * 2;
	mEnemiesSpawned = 0;
	mEnemiesLeft    = mSpawnCount;
	mSpawnTimer.reset();
	mSpawnTimer.setGoal(mSpawnWait);
	
	broadcastMessage("new wave");
}

void WaveBrain::setWave(const int i)
{
	mWave           = 1;
	mSpawnSpeed     = 3.0f;
	mSpawnWait      = 3.0f;
	mSpawnCount     = 3;
	mEnemiesSpawned = 0;
	mEnemiesLeft    = mSpawnCount;
	mSpawnTimer.reset();
	mSpawnTimer.setGoal(mSpawnWait);
	for (int j = 1; j < i; j++)
		incrementWave();
}

int WaveBrain::getWave() const
{
	return mWave;
}
