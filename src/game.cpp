#include "game.hpp"
#include "window.hpp"
#include "clock.hpp"
#include "events.hpp"
#include <iostream>

// "Game" specific data
namespace {
	bool gIsRunning = false;
	
	class GameEventEar : public EventEar {
	public:
		void onWindowClose()
		{
			Game::stop();
		}
	};
	GameEventEar gEar;
}



void Game::run()
{
	// Don't run it twice.
	if (gIsRunning)
		return;
		
	// Initialize the basics
	Window::open(800, 600);
	Clock::setTime(0.0);
	Events::addEar(&gEar);
	
	// Run the game loop
	gIsRunning = true;
	Clock::Timer frameCapTimer;
	while (gIsRunning) 
	{
		// Poll for new events
		Events::poll();
		
		// Perform drawing, logic, etc...
		
		// Cap the framerate at 60
		Clock::sleepFor((1.0 / 60.0) - dt.get());
		frameCapTimer.set(0.0);
	}
	
	// Close the basics
	Events::removeEar(&gEar);
	Window::close();
}

void Game::stop()
{
	gIsRunning = false;
}
