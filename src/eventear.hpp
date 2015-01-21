#pragma once

#include <string>

class EventEar {
public:
	EventEar() {}
	virtual ~EventEar() {}
	
	virtual void onKeyDown(const std::string& s) {}
	virtual void onKeyUp(const std::string& s) {}
	
	virtual void onMouseDown(const std::string& s) {}
	virtual void onMouseUp(const std::string& s) {}
	virtual void onMouseMove(const int x, const int y) {}
	virtual void onMouseScroll(const int i) {}
	
	virtual void onWindowResize(const int w, const int h) {}
	virtual void onWindowClose() {}
};
