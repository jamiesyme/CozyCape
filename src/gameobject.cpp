#include "gameobject.hpp"

GameObject::GameObject()
{
	mGod  = 0;
	mName = std::string();
	mType = std::string();
}

GameObject::~GameObject()
{
}

void GameObject::onInit()
{
	// Do nothing.
}

void GameObject::onKill()
{
	// Do nothing.
}

void GameObject::addListener(GameObject* go)
{
	if (go == 0)
		return;
	for (unsigned int i = 0; i < mListeners.size(); i++)
		if (mListeners[i] == go)
			return;
	mListeners.push_back(go);
}

void GameObject::removeListener(GameObject* go)
{
	if (go == 0)
		return;
	for (unsigned int i = 0; i < mListeners.size(); i++) {
		if (mListeners[i] == go) {
			mListeners.erase(mListeners.begin() + i);
			return;
		}
	}
}

void GameObject::broadcastMessage(const std::string& msg)
{
	for (unsigned int i = 0; i < mListeners.size(); i++)
		mListeners[i]->onMessage(msg, this);
}

void GameObject::onMessage(const std::string& msg, GameObject* go)
{
	// Do nothing.
}

void GameObject::setName(const std::string& name)
{
	mName = name;
}

void GameObject::setType(const std::string& type)
{
	mType = type;
}

std::string GameObject::getName() const
{
	return mName;
}

std::string GameObject::getType() const
{
	return mType;
}

void GameObject::setGod(GameObjectGod* god)
{
	mGod = god;
}

GameObjectGod* GameObject::getGod()
{
	return mGod;
}

TickGod* GameObject::getTickGod()
{
	return mGod->getTickGod();
}

DrawGod* GameObject::getDrawGod()
{
	return mGod->getDrawGod();
}

TriggerGod* GameObject::getTriggerGod()
{
	return mGod->getTriggerGod();
}

PhysicsGod* GameObject::getPhysicsGod()
{
	return mGod->getPhysicsGod();
}
