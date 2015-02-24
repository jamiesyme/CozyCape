#include "ticklayer.hpp"

TickLayer::TickLayer(const std::string& name)
{
	mName = name;
}

TickLayer::~TickLayer()
{
}

std::string TickLayer::getName() const
{
	return mName;
}

void TickLayer::onTick(float dt)
{
	// Update the tickables
	for (unsigned int i = 0; i < mTickables.size(); i++)
		mTickables[i]->onTick(dt);
}

void TickLayer::add(Tickable* t)
{
	if (t != 0)
		mTickables.push_back(t);
}

void TickLayer::remove(Tickable* t)
{
	for (unsigned int i = 0; i < mTickables.size(); i++) {
		if (t == mTickables[i]) {
			mTickables.erase(mTickables.begin() + i);
			return;
		}
	}
}

void TickLayer::removeAll()
{
	mTickables.clear();
}
