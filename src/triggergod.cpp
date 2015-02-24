#include "triggergod.hpp"
#include "events.hpp"

TriggerGod::TriggerGod()
{
	mActiveLayer = 0;
	Events::addEar(this);
}

TriggerGod::~TriggerGod()
{
	for (unsigned int i = 0; i < mLayers.size(); i++)
		delete mLayers[i];
	Events::removeEar(this);
}

void TriggerGod::onKeyDown(const std::string& s)
{
	if (mActiveLayer != 0)
		mActiveLayer->onKeyDown(s);
}

void TriggerGod::onKeyUp(const std::string& s)
{
	if (mActiveLayer != 0)
		mActiveLayer->onKeyUp(s);
}

void TriggerGod::onMouseDown(const std::string& s)
{
	if (mActiveLayer != 0)
		mActiveLayer->onMouseDown(s);
}

void TriggerGod::onMouseUp(const std::string& s)
{
	if (mActiveLayer != 0)
		mActiveLayer->onMouseUp(s);
}

void TriggerGod::onMouseMove(const int x, const int y)
{
	if (mActiveLayer != 0)
		mActiveLayer->onMouseMove(x, y);
}

void TriggerGod::onMouseScroll(const int i)
{
	if (mActiveLayer != 0)
		mActiveLayer->onMouseScroll(i);
}

void TriggerGod::onWindowResize(const int w, const int h)
{
	if (mActiveLayer != 0)
		mActiveLayer->onWindowResize(w, h);
}

void TriggerGod::onWindowClose()
{
	if (mActiveLayer != 0)
		mActiveLayer->onWindowClose();
}

void TriggerGod::addTo(const std::string& layerName, Trigger* t)
{
	getLayer(layerName)->add(t);
}

void TriggerGod::removeFrom(const std::string& layerName, Trigger* t)
{
	getLayer(layerName)->remove(t);
}

TriggerLayer* TriggerGod::addLayer(const std::string& name)
{
	// Check for duplicates
	if (getLayer(name) != 0)
		return 0;
		
	// Add the layer
	TriggerLayer* layer = new TriggerLayer(name);
	mLayers.push_back(layer);
	return layer;
}

TriggerLayer* TriggerGod::getLayer(const std::string& name)
{
	// Find the layer
	for (unsigned int i = 0; i < mLayers.size(); i++)
		if (mLayers[i]->getName() == name)
			return mLayers[i];
			
	// We didn't find it
	return 0;
}

void TriggerGod::removeLayer(const std::string& name)
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

void TriggerGod::setActiveLayer(const std::string& name)
{
	mActiveLayer = getLayer(name);
}

TriggerLayer* TriggerGod::getActiveLayer()
{
	return mActiveLayer;
}
