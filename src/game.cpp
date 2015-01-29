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
#include "image.hpp"
#include "spawner.hpp"

// "Game" specific data
namespace {
	bool gIsRunning = false;
	Camera* gCamera = 0;
	Player* gPlayer = 0;
	
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
	Image levelImg;
	levelImg.load("../res/level1.png");
	const int mapSizeX = levelImg.getSizeX() + 2;
	const int mapSizeY = levelImg.getSizeY() + 2;
	Map::init(mapSizeX, mapSizeY);
	for (int x = 0; x < mapSizeX; x++) {
		Map::setTile(x, 0, 1);
		Map::setTile(x, mapSizeY - 1, 1);
	}
	for (int y = 0; y < mapSizeY; y++) {
		Map::setTile(0, y, 1);
		Map::setTile(mapSizeX - 1, y, 1);
	}
	for (int x = 0; x < mapSizeX - 2; x++)
		for (int y = 0; y < mapSizeY - 2; y++)
			if (levelImg.getPixel(x, y).r < 10)
				Map::setTile(x + 1, y + 1, 1);
	
	// Create the spawner
	Spawner* spawner = new Spawner();
	spawner->addPoint(Vec2(13.5f, 13.5f));
	spawner->addPoint(Vec2(27.5f, 11.5f));
	spawner->addPoint(Vec2( 4.5f, 12.5f));
	spawner->addPoint(Vec2(24.5f, 30.5f));
	Map::manageEntity(spawner);
	
	// Create the player
	Player* player = new Player();
	player->setPos(Vec2(19.0f, 19.0f));
	Map::manageEntity(player);
	Game::setPlayer(player);
	
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

void Game::setPlayer(Player* p)
{
	gPlayer = p;
}

Player* Game::getPlayer()
{
	return gPlayer;
}
