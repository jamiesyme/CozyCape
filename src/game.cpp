#include "game.hpp"
#include "window.hpp"
#include "clock.hpp"
#include "events.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "entity.hpp"
#include "tiles.hpp"
#include "pathfinder.hpp"
#include "player.hpp"
#include "camera.hpp"
#include "spawner.hpp"
#include "commongl.hpp"
#include "raycast.hpp"
#include <vector>
#include <cmath>

// "Game" specific data
namespace Game {
	bool mIsRunning = false;
	Camera* mCamera = 0;
	std::vector<Entity*> mEnts;
	std::vector<int>     mEntStates;
	
	void onTick();
	void onDraw();
	void onPhysics();
	void onStateUpdate();
	
	class WindowCloseEar : public EventEar {
	public:
		void onWindowClose() {
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
	
	// Initialize opengl
	CommonGL::setBgColor(Color::Black);
	
	// Create the tiles
	{
		Tiles* tiles = new Tiles();
		tiles->load("../res/level1.png");
		Game::manageEntity(tiles);
	}
	
	// Create the path finder
	{
		PathFinder* pf = new PathFinder();
		Game::manageEntity(pf);
	}
	
	// Create the spawner
	{
		Spawner* spawner = new Spawner();
		spawner->addPoint(Vec2(13.5f, 13.5f));
		//spawner->addPoint(Vec2(27.5f, 11.5f));
		//spawner->addPoint(Vec2( 4.5f, 12.5f));
		//spawner->addPoint(Vec2(24.5f, 30.5f));
		Game::manageEntity(spawner);
	}
	
	// Create the player
	{
		Player* player = new Player();
		player->setPosition(Vec2(19.0f, 19.0f));
		Game::manageEntity(player);
	}
	
	// Create the camera
	{
		Camera* camera = new Camera(Vec2(16.0f, 12.0f));
		Game::manageEntity(camera);
		Game::setCamera(camera);
		// Make the camera follow the player
		onStateUpdate();
		camera->follow(findEntityByType("player"), 2);
	}
	
	// Run the game loop
	mIsRunning = true;
	Clock::Timer frameCapTimer;
	while (mIsRunning) 
	{
		Events::poll();
		onTick();
		onPhysics();
		onDraw();
		onStateUpdate();
		
		// Cap the framerate at 60
		Clock::sleepFor((1.0 / 60.0) - frameCapTimer.get());
		frameCapTimer.set(0.0);
	}
	
	// Destroy the entities
	for (int i = 0; i < mEnts.size(); i++)
		removeEntity(mEnts[i]);
	onStateUpdate();
	
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

void Game::onTick()
{
	// Update all the (valid) entities
	for (unsigned int i = 0; i < mEnts.size(); i++)
		if (mEntStates[i] == 0)
			mEnts[i]->onTick();
}

void Game::onDraw()
{	
	// Clear opengl
	CommonGL::clearColor();
	if (mCamera != 0)
		mCamera->bind();
	
	// Draw the entities
	for (unsigned int i = 0; i < mEnts.size(); i++)
		if (mEntStates[i] == 0)
			mEnts[i]->onDraw();
	
	// Update the window
	Window::swap();
}

static void onPhysics_solveComplex(Entity* c, Entity* d);
static void onPhysics_solveDynamic(Entity* d1, Entity* d2);
void Game::onPhysics()
{
	// Check all entities against all other entities
	for (int i = 0; i < (int)mEnts.size() - 1; i++) 
	{
		// Skip invalid entities
		if (mEntStates[i] != 0)
			continue;
			
		// Skip empty entities
		Entity* e1 = mEnts[i];
		if (e1->isBodyEmpty())
			continue;
		
		// Loop against the unchecked entities
		for (int j = i + 1; j < (int)mEnts.size(); j++) 
		{
			// Skip invalid entities
			if (mEntStates[j] != 0)
				continue;
				
			// Skip empty entities
			Entity* e2 = mEnts[j];
			if (e2->isBodyEmpty())
				continue;
			
			// If both objects are complex, nothing can happen
			if (e1->isBodyComplex() && e2->isBodyComplex())
				continue;
				
			// If only one object is complex, it will control the outcome
			if (e1->isBodyComplex())
				onPhysics_solveComplex(e1, e2);
			else
			if (e2->isBodyComplex())
				onPhysics_solveComplex(e2, e1);
			
			// Otherwise, both objects are dynamic, so they'll contribute equally
			else
				onPhysics_solveDynamic(e1, e2);
		}
	}
}


void Game::onStateUpdate()
{
	// Update the validity states of the entity
	for (unsigned int i = 0; i < mEntStates.size(); i++) 
	{
		// state < 0 means it was just added
		if (mEntStates[i] < 0)
			mEntStates[i] = 0;
		else
		
		// state > 0 means it was just removed
		if (mEntStates[i] > 1) {
			delete mEnts[i];
			mEnts.erase(mEnts.begin() + i);
			mEntStates.erase(mEntStates.begin() + i);
			i--;
			continue;
		}
	}
}

void Game::manageEntity(Entity* e)
{
	// Skip null entities
	if (e == 0)
		return;
		
	// Check for duplicates
	for (unsigned int i = 0; i < mEnts.size(); i++)
		if (mEnts[i] == e)
			return;
			
	// Manage the entity
	mEnts.push_back(e);
	mEntStates.push_back(-1);
}

void Game::removeEntity(Entity* e)
{
	// Find the entity, and change it's state
	for (unsigned int i = 0; i < mEnts.size(); i++) {
		if (mEnts[i] == e) {
			mEntStates[i] = 1;
			break;
		}
	}
}

Entity* Game::findEntityByType(const std::string& type)
{
	// Find the first valid entity matching 'type'
	for (unsigned int i = 0; i < mEnts.size(); i++)
		if (mEntStates[i] == 0 && mEnts[i]->getType() == type)
			return mEnts[i];
	return 0;
}

static bool doRaycast_circle(Entity* e, const Ray& r, RaycastInfo* i);
bool Game::doRaycast(const Ray& ray, RaycastInfo* info)
{
	bool didHit = false;
	RaycastInfo bestInfo;
	for (unsigned int i = 0; i < mEnts.size(); i++)
	{
		// Skip invalid entities
		if (mEntStates[i] != 0)
			continue;
			
		// Test the raycast
		Entity* e = mEnts[i];
		RaycastInfo tempInfo;
		if (e->isBodyCircle() && doRaycast_circle(e, ray, &tempInfo) ||
		    e->isBodyComplex() && e->doComplexBodyRaycast(ray, &tempInfo))
		{
			// Fill in the missing info
			tempInfo.entity   = e;
			tempInfo.distance = (tempInfo.point - ray.position).length();
			
			// Overwrite the best result
			if (!didHit || tempInfo.distance < bestInfo.distance)
				bestInfo = tempInfo;
				
			// Mark our hit
			didHit = true;
		}
	}
	if (info != 0)
		*info = bestInfo;
	return didHit;
}

void Game::setCamera(Camera* c)
{
	mCamera = c;
}

Camera* Game::getCamera()
{
	return mCamera;
}

static bool doRaycast_circle(Entity* e, const Ray& r, RaycastInfo* i)
{
	// Get some information
	const float rayDirRadians = std::atan2(r.direction.y, r.direction.x);
	const float rayDirCos     = std::cos( rayDirRadians);
	const float rayDirSin     = std::sin( rayDirRadians);
	const float rayDirCosNeg  = std::cos(-rayDirRadians);
	const float rayDirSinNeg  = std::sin(-rayDirRadians);
	const float radius = e->getBodyRadius();

	// Get the transformed pos
	Vec2 tPos = e->getPosition() - r.position;
	tPos = Vec2(tPos.x * rayDirCosNeg - tPos.y * rayDirSinNeg,
	            tPos.x * rayDirSinNeg + tPos.y * rayDirCosNeg);
	
	// Check that it's in front of the ray
	if (tPos.x - radius < 0.0f)
		return false;
		
	// Check that it's overlapping the line
	if (tPos.y - radius > 0.0f ||
	    tPos.y + radius < 0.0f)
	  return false;
	  
	// Calculate the distance to it
	const float dist = tPos.x - std::sqrt(radius * radius - tPos.y * tPos.y);
		
	// Record the info
	if (i != 0) {
		//i->distance = dist;
		i->point    =  Vec2(dist * rayDirCos, dist * rayDirSin) + r.position;
		i->normal   = (Vec2(dist, 0.0f) - tPos).normalized();
		i->normal   =  Vec2(i->normal.x * rayDirCos - i->normal.y * rayDirSin,
			                  i->normal.x * rayDirSin + i->normal.y * rayDirCos);
	}
	return true;
}

static void onPhysics_solveComplex(Entity* c, Entity* d)
{
	// Let the entity handle it
	c->fixComplexBodyCollision(d);
}

static void onPhysics_solveDynamic(Entity* d1, Entity* d2)
{
	// NOTE: Only solves circle vs circle right now!
	if (d1->isBodyCircle() && d2->isBodyCircle()) 
	{
		const float r1  = d1->getBodyRadius();
		const float r2  = d2->getBodyRadius();
		const Vec2 diff = d2->getPosition() - d1->getPosition();
		if (diff.length() < r1 + r2)
		{
			const Vec2 corr = diff.normalized() * (diff.length() - (r1 + r2));
			d1->translate( corr / 2);
			d2->translate(-corr / 2);
		}
	}	
}
