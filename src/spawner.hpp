#pragma once

#include "entity.hpp"
#include "clock.hpp"
#include <vector>

class Spawner : public Entity {
public:
	Spawner();
	~Spawner();
	
	void onTick();
	void onDraw();
	
	void addPoint(const Vec2& p);

private:
	std::vector<Vec2> mPoints;
	Clock::Timer mSpawnTimer;
};
