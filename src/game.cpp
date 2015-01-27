#include "game.hpp"
#include "window.hpp"
#include "clock.hpp"
#include "events.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "camera.hpp"
#include "map.hpp"
#include "commongl.hpp"

// "Game" specific data
namespace {
	bool gIsRunning = false;
	Camera* gCamera = 0;
	
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
	Mouse::init();
	
	// Initialize opengl
	CommonGL::setBgColor(Color::Black);
	
	// Init the game map
	Map::init(20, 20);
	Map::setTile(5, 5, 1);
	Map::setTile(1, 1, 1);
	
	// Create the player
	Player* player = new Player();
	player->setPos(Vec2(3.0f, 2.0f));
	Map::manageEntity(player);
	
	// Create the camera
	Camera* camera = new Camera(Vec2(16.0f, 12.0f));
	camera->setSolid(false);
	camera->setRadius(2.0f);
	camera->follow(player);
	Map::manageEntity(camera);
	Game::setCamera(camera);
	
	// Run the game loop
	gIsRunning = true;
	Clock::Timer frameCapTimer;
	while (gIsRunning) 
	{
		// Poll for new events
		Events::poll();
		
		// Game Logic
		Map::onTick();
		
		// Game Physics
		Map::onPhysics();
		
		// Game Drawing
		CommonGL::clearColor();
		if (gCamera != 0)
			gCamera->bind();
		Map::onDraw();
		Window::swap();
		
		// Cap the framerate at 60
		Clock::sleepFor((1.0 / 60.0) - frameCapTimer.get());
		frameCapTimer.set(0.0);
	}
	
	// Close the basics
	Map::kill();
	Mouse::kill();
	Keyboard::kill();
	Events::removeEar(&gEar);
	Window::close();
}

void Game::stop()
{
	gIsRunning = false;
}

void Game::setCamera(Camera* c)
{
	gCamera = c;
}

Camera* Game::getCamera()
{
	return gCamera;
}
