#pragma once

#include <vector>
#include "gameobject.hpp"
#include "tickable.hpp"
#include "counter.hpp"
#include "vec2.hpp"

class Spawner : public GameObject,
                public Tickable {
public:
	Spawner();
	~Spawner();
	
	void onInit();
	void onKill();
	void onTick(float dt);
	
	void addPoint(const Vec2& p);

private:
	std::vector<Vec2> mPoints;
	Counter mSpawnTimer;
};
