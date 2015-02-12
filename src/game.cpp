#include "game.hpp"
#include "window.hpp"
#include "clock.hpp"
#include "events.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "entitymanager.hpp"
#include "tiles.hpp"
#include "pathfinder.hpp"
#include "player.hpp"
#include "camera.hpp"
#include "spawner.hpp"
#include "commongl.hpp"

// "Game" specific data
namespace Game {
	bool mIsRunning = false;
	Camera* mCamera = 0;
	EntityManager* mEntMgr;
	
	class WindowCloseEar : public EventEar {
	public:
		void onWindowClose() 
		{
			Game::stop();
		}
	};
	WindowCloseEar mWindowCloseEar;
}



void Game::run()
{
	// Don't run it twice.
	if (mIsRunning)
		return;
		
	// Initialize the basics
	Window::open(800, 600);
	Clock::setTime(0.0);
	Events::addEar(&mWindowCloseEar);
	Keyboard::init();
	Mouse::init();
	
	// Create the entity manager
	mEntMgr = new EntityManager();
	
	// Initialize opengl
	CommonGL::setBgColor(Color::Black);
	CommonGL::enableAlphaBlend();
	CommonGL::enableDepthTest();
	
	// Create the tiles
	{
		Tiles* tiles = new Tiles();
		tiles->load("../res/level1.png");
		mEntMgr->manage(tiles);
	}
	
	// Create the path finder
	{
		PathFinder* pf = new PathFinder();
		mEntMgr->manage(pf);
	}
	
	// Create the spawner
	{
		Spawner* spawner = new Spawner();
		spawner->addPoint(Vec2(13.5f, 13.5f));
		spawner->addPoint(Vec2(27.5f, 11.5f));
		spawner->addPoint(Vec2( 4.5f, 12.5f));
		spawner->addPoint(Vec2(24.5f, 30.5f));
		mEntMgr->manage(spawner);
	}
	
	// Create the player
	{
		Player* player = new Player();
		player->setPosition(Vec2(19.0f, 19.0f));
		mEntMgr->manage(player);
		
		// Create the camera
		{
			Camera* camera = new Camera(Vec2(16.0f, 12.0f));
			camera->follow(player, 2);
			mEntMgr->manage(camera);
			Game::setCamera(camera);
		}
	}
	
	// Run the game loop
	mIsRunning = true;
	Clock::Timer frameCapTimer;
	while (mIsRunning) 
	{
		// Send out new events
		Events::poll();
		
		// Update the game state
		mEntMgr->onTick();
		mEntMgr->onPhysics();
		
		// Draw
		CommonGL::clearColorAndDepth();
		if (mCamera != 0)
			mCamera->bind();
		mEntMgr->onDraw();
		Window::swap();
		
		// Cap the framerate at 60
		Clock::sleepFor((1.0 / 60.0) - frameCapTimer.get());
		frameCapTimer.set(0.0);
	}
	
	// Destroy the entity manager
	delete mEntMgr;
	mEntMgr = 0;

	// Close the basics
	Mouse::kill();
	Keyboard::kill();
	Events::removeEar(&mWindowCloseEar);
	Window::close();
}

void Game::stop()
{
	mIsRunning = false;
}

void Game::setCamera(Camera* c)
{
	mCamera = c;
}

Camera* Game::getCamera()
{
	return mCamera;
}

EntityManager* Game::getEntityManager() 
{
	return mEntMgr;
}
