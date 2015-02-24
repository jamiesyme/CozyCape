#pragma once

#include <vector>
#include <string>
#include "drawlayer.hpp"
#include "camera.hpp"

class DrawGod {
public:
	DrawGod();
	~DrawGod();
	
	void onDraw();
	
	void      addTo(const std::string& layerName, Drawable* d);
	void removeFrom(const std::string& layerName, Drawable* d);
	
	Camera* addCamera(const std::string& name);
	Camera* getCamera(const std::string& name);
	void removeCamera(const std::string& name);
	
	DrawLayer* addLayer(const std::string& name);
	DrawLayer* getLayer(const std::string& name);
	void    removeLayer(const std::string& name);
	
	void       setActiveLayer(const std::string& name);
	DrawLayer* getActiveLayer();

private:
	std::vector<DrawLayer*> mLayers;
	std::vector<Camera*> mCameras;
	DrawLayer* mActiveLayer;
};
