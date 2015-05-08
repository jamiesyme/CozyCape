#pragma once

#include "gameobject.hpp"
#include "tickable.hpp"
#include "counter.hpp"

class WaveBrain : public GameObject,
                  public Tickable {
public:
	WaveBrain();
	~WaveBrain();
	
	void onInit();
	void onKill();
	void onTick(float dt);
	void onMessage(const std::string& msg, GameObject* go);
	
	void incrementWave();
	void setWave(const int wave);
	int  getWave() const;
	
private:
	int     mWave;
	float   mSpawnSpeed;
	float   mSpawnWait;
	int     mSpawnCount;
	Counter mSpawnTimer;
	int     mEnemiesSpawned;
	int     mEnemiesLeft;
	
	class Spawner;
	std::vector<Spawner> mSpawners;
};
