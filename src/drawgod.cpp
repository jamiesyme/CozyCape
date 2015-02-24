#include "drawgod.hpp"

DrawGod::DrawGod()
{
	mActiveLayer = 0;
}

DrawGod::~DrawGod()
{
	for (unsigned int i = 0; i < mLayers.size(); i++)
		delete mLayers[i];
	for (unsigned int i = 0; i < mCameras.size(); i++)
		delete mCameras[i];
}

void DrawGod::onDraw()
{
	if (mActiveLayer != 0)
		mActiveLayer->onDraw();
}

void DrawGod::addTo(const std::string& layerName, Drawable* d)
{
	getLayer(layerName)->add(d);
}

void DrawGod::removeFrom(const std::string& layerName, Drawable* d)
{
	getLayer(layerName)->remove(d);
}

Camera* DrawGod::addCamera(const std::string& name)
{
	// Check for duplicates
	if (getCamera(name) != 0)
		return 0;
		
	// Add the camera
	Camera* cam = new Camera(name);
	mCameras.push_back(cam);
	return cam;
}

Camera* DrawGod::getCamera(const std::string& name)
{
	// Find the cammera
	for (unsigned int i = 0; i < mCameras.size(); i++)
		if (mCameras[i]->getName() == name)
			return mCameras[i];
			
	// We didn't find it
	return 0;
}

void DrawGod::removeCamera(const std::string& name)
{
	// Find and destroy the camera
	for (unsigned int i = 0; i < mCameras.size(); i++) {
		if (mCameras[i]->getName() == name) {
			delete mCameras[i];
			mCameras.erase(mCameras.begin() + i);
			return;
		}
	}
}

DrawLayer* DrawGod::addLayer(const std::string& name)
{
	// Check for duplicates
	if (getLayer(name) != 0)
		return 0;
		
	// Add the layer
	DrawLayer* layer = new DrawLayer(name);
	mLayers.push_back(layer);
	return layer;
}

DrawLayer* DrawGod::getLayer(const std::string& name)
{
	// Find the layer
	for (unsigned int i = 0; i < mLayers.size(); i++)
		if (mLayers[i]->getName() == name)
			return mLayers[i];
			
	// We didn't find it
	return 0;
}

void DrawGod::removeLayer(const std::string& name)
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

void DrawGod::setActiveLayer(const std::string& name)
{
	mActiveLayer = getLayer(name);
}

DrawLayer* DrawGod::getActiveLayer()
{
	return mActiveLayer;
}
