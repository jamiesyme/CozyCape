#include "game.hpp"
#include "window.hpp"
#include "clock.hpp"
#include "events.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "gameobjectgod.hpp"
#include "tiles.hpp"
#include "pathfinder.hpp"
#include "player.hpp"
#include "wavebrain.hpp"
#include "wavehud.hpp"
#include "camerafollower.hpp"
#include "healthhud.hpp"
#include "commongl.hpp"
#include "fonthelper.hpp"
#include "discobackground.hpp"
#include <iostream>

// "Game" specific data
namespace Game {
	bool mIsRunning = false;
	GameObjectGod* mGoGod;
	
	class WindowCloseEar : public EventEar {
	public:
		void onWindowClose() 
		{ Game::stop(); }                    
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
	FontHelper::init();
	FontHelper::setAlias("../res/VeraMono.ttf", "Vera");
	
	// Initialize opengl
	CommonGL::setBgColor(Color(0.4f, 0.1f, 0.1f));
	CommonGL::enableAlphaBlend();
	CommonGL::enableTextures();
	
	// Create the game object god
	mGoGod = new GameObjectGod();
	
	{ // Create the layers
		{ // Tick Layers
			TickGod* tickGod = mGoGod->getTickGod();
			TickLayer* l1 = tickGod->addLayer("enemies");
			TickLayer* l2 = tickGod->addLayer("players");
			TickLayer* l3 = tickGod->addLayer("weapons");
			TickLayer* l4 = tickGod->addLayer("spawners");
			TickLayer* l5 = tickGod->addLayer("game");
			TickLayer* l6 = tickGod->addLayer("physics");
			TickLayer* l7 = tickGod->addLayer("post");
			TickLayer* l8 = tickGod->addLayer("hud");
			l5->add(l1);
			l5->add(l2);
			l5->add(l3);
			l5->add(l4);
			l5->add(l6);
			l5->add(l7);
			l5->add(l8);
			tickGod->setActiveLayer("game");
			
			// HACK FOR PHYSICS
			l6->add(mGoGod->getPhysicsGod());
		}
		
		{ // Draw Layers
			DrawGod* drawGod = mGoGod->getDrawGod();
			Camera* c1 = drawGod->addCamera("main");
			Camera* c2 = drawGod->addCamera("screen");
			c1->set(Vec2(0.0f, 0.0f), Vec2(16.0f, 12.0f));
			c2->set(Vec2(0.0f, 0.0f), Vec2(800.0f, 600.0f));
			DrawLayer* l1 = drawGod->addLayer("enemies");
			DrawLayer* l2 = drawGod->addLayer("players");
			DrawLayer* l3 = drawGod->addLayer("weapons");
			DrawLayer* l4 = drawGod->addLayer("map");
			DrawLayer* l5 = drawGod->addLayer("game");
			DrawLayer* l6 = drawGod->addLayer("post");
			DrawLayer* l7 = drawGod->addLayer("hud");
			l5->add(l1, 0.8f);
			l5->add(l2, 0.7f);
			l5->add(l3, 0.75f);
			l5->add(l4, 0.9f);
			l5->add(l6, 0.4f);
			l5->add(l7, 0.3f);
			l5->setCamera(c1);
			l7->setCamera(c2);
			drawGod->setActiveLayer("game");
		}
		
		{ // Trigger Layers
			TriggerGod* triggerGod = mGoGod->getTriggerGod();
			TriggerLayer* l1 = triggerGod->addLayer("game");
			triggerGod->setActiveLayer("game");
		}
	}
	
	{ // Create the tiles
		Tiles* tiles = new Tiles();
		tiles->load("../res/level1.png");
		mGoGod->manage(tiles);
	}
	
	{ // Create the path finder
		PathFinder* pf = new PathFinder();
		mGoGod->manage(pf);
	}
	
	{ // Create the wave brain
		WaveBrain* waveBrain = new WaveBrain();
		mGoGod->manage(waveBrain);
		
		{ // Create the wave hud
			WaveHud* wh = new WaveHud();
			mGoGod->manage(wh);
		}
	}
	
	{ // Create the player
		Player* player = new Player();
		player->setPosition(Vec2(19.0f, 19.0f));
		mGoGod->manage(player);
		
		{ // Create the camera follower
			Camera* cam = mGoGod->getDrawGod()->getCamera("main");
			CameraFollower* cf = new CameraFollower(cam);
			cf->follow(player);
			cf->setRadius(1.0f);
			mGoGod->manage(cf);
		}
		{ // Create the health hud
			HealthHud* hh = new HealthHud(player);
			mGoGod->manage(hh);
		}
	}
	
	{ // Create the disco background
		mGoGod->manage(new DiscoBackground());
	}
	
	// Run the game loop
	mIsRunning = true;
	Clock::Timer frameCapTimer;
	while (mIsRunning) 
	{
		// Check for events/triggers
		Events::poll();
		
		// Update the game state
		mGoGod->getTickGod()->onTick();
		
		// Update our game objects
		mGoGod->update();
		
		// Draw the game
		CommonGL::clearColor();
		mGoGod->getDrawGod()->onDraw();
		Window::swap();
		
		// Cap the framerate at 60
		Clock::sleepFor((1.0 / 60.0) - frameCapTimer.get());
		frameCapTimer.set(0.0);
	}
	
	// Destroy the game object god
	delete mGoGod;
	mGoGod = 0;

	// Close the basics
	FontHelper::kill();
	Mouse::kill();
	Keyboard::kill();
	Events::removeEar(&mWindowCloseEar);
	Window::close();
}

void Game::stop()
{
	mIsRunning = false;
}
