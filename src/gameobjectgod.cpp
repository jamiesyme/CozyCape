#include "gameobjectgod.hpp"
#include "gameobject.hpp"
#include "tickgod.hpp"
#include "drawgod.hpp"
#include "triggergod.hpp"
#include "physicsgod.hpp"

GameObjectGod::GameObjectGod() 
{
	// Create the gods
	mTickGod = new TickGod();
	mDrawGod = new DrawGod();
	mTriggerGod = new TriggerGod();
	mPhysicsGod = new PhysicsGod();
}

GameObjectGod::~GameObjectGod()
{
	// Remove all the remaining game objects
	for (unsigned int i = 0; i < mGos.size(); i++)
		mGosToRemove.push_back(mGos[i]);
	for (unsigned int i = 0; i < mGosToAdd.size(); i++)
		mGosToRemove.push_back(mGosToAdd[i]);
	mGos.clear();
	mGosToAdd.clear();
	update();
	
	// Delete the gods
	delete mTickGod;
	delete mDrawGod;
	delete mTriggerGod;
	delete mPhysicsGod;
}

void GameObjectGod::update()
{
	// Add game objects
	for (unsigned int i = 0; i < mGosToAdd.size(); i++) {
		mGos.push_back(mGosToAdd[i]);
		mGosToAdd[i]->onInit();
	}
	mGosToAdd.clear();
	
	// Remove game objects
	for (unsigned int i = 0; i < mGosToRemove.size(); i++) {
		mGosToRemove[i]->onKill();
		delete mGosToRemove[i];
	}
	mGosToRemove.clear();
}

void GameObjectGod::manage(GameObject* go)
{
	// Skip null game objects
	if (go == 0)
		return;
	
	// Add the GameObject
	go->setGod(this);
	mGosToAdd.push_back(go);
}

void GameObjectGod::remove(GameObject* go)
{
	// Skip null game objects
	if (go == 0)
		return;
		
	// Find the GameObject, and move it
	for (unsigned int i = 0; i < mGos.size(); i++) {
		if (go == mGos[i]) {
			mGosToRemove.push_back(go);
			mGos.erase(mGos.begin() + i);
			return;
		}
	}
}

GameObject* GameObjectGod::findByType(const std::string& type)
{
	// Search all the game objects
	for (unsigned int i = 0; i < mGos.size(); i++) 
		if (mGos[i]->getType() == type)
			return mGos[i];
	
	// We didn't find it
	return 0;
}

GameObject* GameObjectGod::findByName(const std::string& name)
{
	// Search all the game objects
	for (unsigned int i = 0; i < mGos.size(); i++) 
		if (mGos[i]->getName() == name)
			return mGos[i];
	
	// We didn't find it
	return 0;
}

bool GameObjectGod::doRaycast(Raycast& ray)
{
	return mPhysicsGod->doRaycast(ray);
}

bool GameObjectGod::doRaycast(Raycast& ray, const float maxDist)
{
	return mPhysicsGod->doRaycast(ray, maxDist);
}

TickGod* GameObjectGod::getTickGod()
{
	return mTickGod;
}

DrawGod* GameObjectGod::getDrawGod()
{
	return mDrawGod;
}

TriggerGod* GameObjectGod::getTriggerGod()
{
	return mTriggerGod;
}

PhysicsGod* GameObjectGod::getPhysicsGod()
{
	return mPhysicsGod;
}
