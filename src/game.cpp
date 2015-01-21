#include "game.hpp"
#include "window.hpp"
#include "clock.hpp"
#include "events.hpp"
#include "entity.hpp"
#include "commongl.hpp"
#include <vector>

// "Game" specific data
namespace {
	bool gIsRunning = false;
	std::vector<Entity*> gEnts;
	
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
	
	// Initialize opengl
	CommonGL::setOrtho(Vec2(0.0f, 16.0f),
										 Vec2(12.0f, 0.0f),
										 Vec2(-1.0f, 1.0f));
	CommonGL::setBgColor(Color::White);
	
	// Create the player
	Entity* player = new Entity();
	player->setPos(Vec2(3.0f, 2.0f));
	Game::manageEntity(player);
	
	// Run the game loop
	gIsRunning = true;
	Clock::Timer frameCapTimer;
	while (gIsRunning) 
	{
		// Poll for new events
		Events::poll();
		
		// Game Logic
		for (unsigned int i = 0; i < gEnts.size(); i++)
			gEnts[i]->onTick();
		
		// Game Drawing
		CommonGL::clearColor();
		for (unsigned int i = 0; i < gEnts.size(); i++)
			gEnts[i]->onDraw();
		Window::swap();
		
		// Cap the framerate at 60
		Clock::sleepFor((1.0 / 60.0) - frameCapTimer.get());
		frameCapTimer.set(0.0);
	}
	
	// Close the basics
	for (unsigned int i = 0; i < gEnts.size(); i++)
		delete gEnts[i];
	gEnts.clear();
	Events::removeEar(&gEar);
	Window::close();
}

void Game::stop()
{
	gIsRunning = false;
}

void Game::manageEntity(Entity* e)
{
	// Skip null entities
	if (e == 0)
		return;
		
	// Check for duplicates
	for (unsigned int i = 0; i < gEnts.size(); i++)
		if (gEnts[i] == e)
			return;
			
	// Manage the entity
	gEnts.push_back(e);
}

void Game::destroyEntity(Entity* e)
{
	// Skip null entities
	if (e == 0)
		return;
		
	// Locate and destroy the entity
	for (unsigned int i = 0; i < gEnts.size(); i++)
	{
		if (gEnts[i] == e) 
		{
			delete gEnts[i];
			gEnts.erase(gEnts.begin() + i);
			break;
		}
	}
}
