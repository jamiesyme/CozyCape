#include "gameobject.hpp"
#include "gameobjectgod.hpp"

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

void GameObject::onMessage(const std::string& s, void* d)
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
