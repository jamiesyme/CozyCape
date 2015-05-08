#pragma once

#include <string>
#include <vector>
#include "gameobjectgod.hpp"

class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	
	virtual void onInit();
	virtual void onKill();
	
	void    addListener(GameObject* go);
	void removeListener(GameObject* go);
	void broadcastMessage(const std::string& msg);
	virtual void onMessage(const std::string& msg, GameObject* go);
	
	void setName(const std::string& name);
	void setType(const std::string& type);
	std::string getName() const;
	std::string getType() const;
	
	void setGod(GameObjectGod* god);
	GameObjectGod* getGod();
	
	TickGod*    getTickGod();
	DrawGod*    getDrawGod();
	TriggerGod* getTriggerGod();
	PhysicsGod* getPhysicsGod();

private:
	GameObjectGod* mGod;
	std::string mName;
	std::string mType;
	std::vector<GameObject*> mListeners;
};
