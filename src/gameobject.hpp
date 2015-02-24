#pragma once

#include <string>
class GameObjectGod;
class TickGod;
class DrawGod;
class TriggerGod;
class PhysicsGod;

class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	
	virtual void onInit();
	virtual void onKill();
	virtual void onMessage(const std::string& s, void* d);
	
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
};
