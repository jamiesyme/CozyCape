#pragma once

#include "eventear.hpp"
#include <vector>
#include <string>

class EventSpeaker : public EventEar {
public:
	EventSpeaker();
	virtual ~EventSpeaker();
	
	void    addEar(EventEar* e);
	void removeEar(EventEar* e);	
	
	virtual void onKeyDown(const std::string& s);
	virtual void onKeyUp(const std::string& s);
	
	virtual void onMouseDown(const std::string& s);
	virtual void onMouseUp(const std::string& s);
	virtual void onMouseMove(const int x, const int y);
	virtual void onMouseScroll(const int i);
	
	virtual void onWindowResize(const int w, const int h);
	virtual void onWindowClose();
	
private:
	std::vector<EventEar*> mEars;
};
