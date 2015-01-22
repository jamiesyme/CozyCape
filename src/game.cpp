#include "game.hpp"
#include "window.hpp"
#include "clock.hpp"
#include "events.hpp"
#include "keyboard.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "map.hpp"
#include "commongl.hpp"

// "Game" specific data
namespace {
	bool gIsRunning = false;
	
	class GameEventEar : public EventEar {
	public:
		void onWindowClose() {
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
	Keyboard::init();
	
	// Initialize opengl
	CommonGL::setOrtho(Vec2(0.0f, 16.0f),
										 Vec2(12.0f, 0.0f),
										 Vec2(-1.0f, 1.0f));
	CommonGL::setBgColor(Color::White);
	
	// Init the game map
	Map::init(20, 20);
	Map::setTile(5, 5, 1);
	
	// -- Create the player
	Entity* player = new Player();
	player->setPos(Vec2(3.0f, 2.0f));
	Map::manageEntity(player);
	
	// Run the game loop
	gIsRunning = true;
	Clock::Timer frameCapTimer;
	while (gIsRunning) 
	{
		// Poll for new events
		Events::poll();
		
		// Game Logic
		Map::onTick();
		
		// Game Drawing
		CommonGL::clearColor();
		Map::onDraw();
		Window::swap();
		
		// Cap the framerate at 60
		Clock::sleepFor((1.0 / 60.0) - frameCapTimer.get());
		frameCapTimer.set(0.0);
	}
	
	// Close the basics
	Map::kill();
	Keyboard::kill();
	Events::removeEar(&gEar);
	Window::close();
}

void Game::stop()
{
	gIsRunning = false;
}
