#include "eventspeaker.hpp"

EventSpeaker::EventSpeaker()
{
}

EventSpeaker::~EventSpeaker()
{
}

void EventSpeaker::addEar(EventEar* e)
{
	// Skip null ears
	if (e == 0)
		return;
		
	// Skip the ear if we already have it
	for (unsigned int i = 0; i < mEars.size(); i++)
		if (mEars[i] == e)
			return;
			
	// Store the ear
	mEars.push_back(e);
}

void EventSpeaker::removeEar(EventEar* e)
{
	// Search for the ear and remove it
	for (unsigned int i = 0; i < mEars.size(); i++) {
		if (mEars[i] == e) {
			mEars.erase(mEars.begin() + i);
			return;
		}
	}
}

void EventSpeaker::onKeyDown(const std::string& s) 
{
	for (unsigned int i = 0; i < mEars.size(); i++)
		mEars[i]->onKeyDown(s);
}

void EventSpeaker::onKeyUp(const std::string& s) 
{
	for (unsigned int i = 0; i < mEars.size(); i++)
		mEars[i]->onKeyUp(s);
}

void EventSpeaker::onMouseDown(const std::string& s) 
{
	for (unsigned int i = 0; i < mEars.size(); i++)
		mEars[i]->onMouseDown(s);
}

void EventSpeaker::onMouseUp(const std::string& s) 
{
	for (unsigned int i = 0; i < mEars.size(); i++)
		mEars[i]->onMouseUp(s);
}

void EventSpeaker::onMouseMove(const int x, const int y) 
{
	for (unsigned int i = 0; i < mEars.size(); i++)
		mEars[i]->onMouseMove(x, y);
}

void EventSpeaker::onMouseScroll(const int i) 
{
	for (unsigned int i = 0; i < mEars.size(); i++)
		mEars[i]->onMouseScroll(i);
}

void EventSpeaker::onWindowResize(const int w, const int h) 
{
	for (unsigned int i = 0; i < mEars.size(); i++)
		mEars[i]->onWindowResize(w, h);
}

void EventSpeaker::onWindowClose() 
{
	for (unsigned int i = 0; i < mEars.size(); i++)
		mEars[i]->onWindowClose();
}
