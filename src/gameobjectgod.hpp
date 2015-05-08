#pragma once

#include <string>
#include <vector>
#include "raycast.hpp"
#include "tickgod.hpp"
#include "drawgod.hpp"
#include "triggergod.hpp"
#include "physicsgod.hpp"
class GameObject;

class GameObjectGod {
public:
	GameObjectGod();
	~GameObjectGod();
	
	void update();
	void manage(GameObject* go);
	void remove(GameObject* go);
	
	GameObject* findByType(const std::string& type);
	GameObject* findByName(const std::string& name);
	
	bool doRaycast(Raycast& ray);
	bool doRaycast(Raycast& ray, const float maxDist);
	
	TickGod*    getTickGod();
	DrawGod*    getDrawGod();
	TriggerGod* getTriggerGod();
	PhysicsGod* getPhysicsGod();

private:
	std::vector<GameObject*> mGos;
	std::vector<GameObject*> mGosToAdd;
	std::vector<GameObject*> mGosToRemove;
	TickGod*    mTickGod;
	DrawGod*    mDrawGod;
	TriggerGod* mTriggerGod;
	PhysicsGod* mPhysicsGod;
};
