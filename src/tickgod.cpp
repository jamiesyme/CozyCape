#include "tickgod.hpp"

TickGod::TickGod()
{
	mActiveLayer = 0;
}

TickGod::~TickGod()
{
	for (unsigned int i = 0; i < mLayers.size(); i++)
		delete mLayers[i];
}

void TickGod::onTick()
{
	if (mActiveLayer != 0)
		mActiveLayer->onTick(1.0f / 60.0f);
}

void TickGod::addTo(const std::string& layerName, Tickable* t)
{
	getLayer(layerName)->add(t);
}

void TickGod::removeFrom(const std::string& layerName, Tickable* t)
{
	getLayer(layerName)->remove(t);
}

TickLayer* TickGod::addLayer(const std::string& name)
{
	// Check for duplicates
	if (getLayer(name) != 0)
		return 0;
		
	// Add the layer
	TickLayer* layer = new TickLayer(name);
	mLayers.push_back(layer);
	return layer;
}

TickLayer* TickGod::getLayer(const std::string& name)
{
	// Find the layer
	for (unsigned int i = 0; i < mLayers.size(); i++)
		if (mLayers[i]->getName() == name)
			return mLayers[i];
			
	// We didn't find it
	return 0;
}

void TickGod::removeLayer(const std::string& name)
{
	// Find and destroy the layer
	for (unsigned int i = 0; i < mLayers.size(); i++) {
		if (mLayers[i]->getName() == name) {
			delete mLayers[i];
			mLayers.erase(mLayers.begin() + i);
			return;
		}
	}
}

void TickGod::setActiveLayer(const std::string& name)
{
	mActiveLayer = getLayer(name);
}

TickLayer* TickGod::getActiveLayer()
{
	return mActiveLayer;
}
