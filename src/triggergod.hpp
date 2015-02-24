#pragma once

#include <vector>
#include <string>
#include "triggerlayer.hpp"
#include "eventear.hpp"

class TriggerGod : public EventEar {
public:
	TriggerGod();
	~TriggerGod();
	
	void onKeyDown(const std::string& s);
	void onKeyUp(const std::string& s);
	
	void onMouseDown(const std::string& s);
	void onMouseUp(const std::string& s);
	void onMouseMove(const int x, const int y);
	void onMouseScroll(const int i);
	
	void onWindowResize(const int w, const int h);
	void onWindowClose();
	
	void      addTo(const std::string& layerName, Trigger* t);
	void removeFrom(const std::string& layerName, Trigger* t);
	
	TriggerLayer* addLayer(const std::string& name);
	TriggerLayer* getLayer(const std::string& name);
	void       removeLayer(const std::string& name);
	
	void          setActiveLayer(const std::string& name);
	TriggerLayer* getActiveLayer();

private:
	std::vector<TriggerLayer*> mLayers;
	TriggerLayer* mActiveLayer;
};
