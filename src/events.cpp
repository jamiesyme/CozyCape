#include "events.hpp"
#include "eventspeaker.hpp"
#include <SDL2/SDL.h>

namespace {
	EventSpeaker gSpeaker;
	
	std::string getKey(SDL_Keysym keySym)
	{
		// a - z
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
	
	std::string getButton(int sdlButton)
	{
		if (sdlButton == SDL_BUTTON_LEFT)
			return "left";
		if (sdlButton == SDL_BUTTON_RIGHT)
			return "right";
		if (sdlButton == SDL_BUTTON_MIDDLE)
			return "middle";
			
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
			
		case SDL_MOUSEBUTTONDOWN:
			text = getButton(sdlEvent.button.button);
			if (text.size() > 0)
				gSpeaker.onMouseDown(text);
			break;
			
		case SDL_MOUSEBUTTONUP:
			text = getButton(sdlEvent.button.button);
			if (text.size() > 0)
				gSpeaker.onMouseUp(text);
			break;
			
		case SDL_MOUSEMOTION:
			gSpeaker.onMouseMove(sdlEvent.motion.x, sdlEvent.motion.y);
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
