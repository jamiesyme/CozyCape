#pragma once

#include <vector>
#include <string>
#include "trigger.hpp"

class TriggerLayer : public Trigger {
public:
	TriggerLayer(const std::string& name);
	~TriggerLayer();
	
	std::string getName() const;
	
	void onKeyDown(const std::string& s);
	void onKeyUp(const std::string& s);
	
	void onMouseDown(const std::string& s);
	void onMouseUp(const std::string& s);
	void onMouseMove(const int x, const int y);
	void onMouseScroll(const int i);
	
	void onWindowResize(const int w, const int h);
	void onWindowClose();
	
	void    add(Trigger* t);
	void remove(Trigger* t);
	void removeAll();

private:
	std::string mName;
	std::vector<Trigger*> mTriggers;
};
