#include "events.hpp"
#include "eventspeaker.hpp"
#include <SDL2/SDL.h>

namespace {
	EventSpeaker gSpeaker;
	
	std::string getKey(SDL_Keysym keySym)
	{
		// A - Z
		if (keySym.sym >= SDLK_a && keySym.sym <= SDLK_z)
			return std::string(1, (char)('a' + (keySym.sym - SDLK_a)));
			
		// 0 - 9
		if (keySym.sym >= SDLK_0 && keySym.sym <= SDLK_9)
			return std::string(1, (char)('0' + (keySym.sym - SDLK_0)));
			
		// Specials
		if (keySym.sym == SDLK_ESCAPE)
			return "escape";
			
		return "";
	}
}


void Events::poll()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent) != 0) 
	{
		// This string is used in the switch cases.
		//   It has to be declared outside of the switch statement.
		std::string text;
		
		// Handle the event
		switch(sdlEvent.type) {
		case SDL_KEYDOWN:
			text = getKey(sdlEvent.key.keysym);
			if (text.size() > 0)
				gSpeaker.onKeyDown(text);
			break;
			
		case SDL_KEYUP:
			text = getKey(sdlEvent.key.keysym);
			if (text.size() > 0)
				gSpeaker.onKeyUp(text);
			break;
			
		case SDL_QUIT:
			gSpeaker.onWindowClose();
			break;
		}
	}
}

void Events::addEar(EventEar* e)
{
	gSpeaker.addEar(e);
}

void Events::removeEar(EventEar* e)
{
	gSpeaker.removeEar(e);
}
