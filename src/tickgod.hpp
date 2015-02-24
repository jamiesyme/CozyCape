#pragma once

#include <vector>
#include <string>
#include "ticklayer.hpp"

class TickGod {
public:
	TickGod();
	~TickGod();
	
	void onTick();
	
	void      addTo(const std::string& layerName, Tickable* t);
	void removeFrom(const std::string& layerName, Tickable* t);
	
	TickLayer* addLayer(const std::string& name);
	TickLayer* getLayer(const std::string& name);
	void    removeLayer(const std::string& name);
	
	void       setActiveLayer(const std::string& name);
	TickLayer* getActiveLayer();

private:
	std::vector<TickLayer*> mLayers;
	TickLayer* mActiveLayer;
};
