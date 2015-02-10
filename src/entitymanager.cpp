#include "entitymanager.hpp"
#include "entity.hpp"
#include "raycast.hpp"
#include <cmath>

EntityManager::EntityManager() 
{
}

EntityManager::~EntityManager()
{
	// Remove all the remaining entities
	for (unsigned int i = 0; i < mEnts.size(); i++)
		remove(mEnts[i]);
	_onStateTick();
}

void EntityManager::manage(Entity* e)
{
	// Skip null or duplicate entities
	if (e == 0)
		return;
	for (unsigned int i = 0; i < mEnts.size(); i++)
		if (e == mEnts[i])
			return;
	
	// Add the entity
	e->setManager(this);
	mEnts.push_back(e);
	mEntStates.push_back(-1);
}

void EntityManager::remove(Entity* e)
{
	// Skip null entities
	if (e == 0)
		return;
		
	// Find the entity, and update it's state
	for (unsigned int i = 0; i < mEnts.size(); i++) {
		if (e == mEnts[i]) {
			mEntStates[i] = 1;
			return;
		}
	}
}

Entity* EntityManager::findByType(const std::string& type)
{
	// Search all the (valid) entities
	for (unsigned int i = 0; i < mEnts.size(); i++) 
		if (mEntStates[i] == 0 && mEnts[i]->getType() == type)
			return mEnts[i];
	
	// We didn't find it
	return 0;
}

void EntityManager::onTick()
{
	// Update all the (valid) entities
	for (unsigned int i = 0; i < mEnts.size(); i++)
		if (mEntStates[i] == 0)
			mEnts[i]->onTick();
			
	// Update the entities with an abnormal state
	_onStateTick();
}

void EntityManager::onDraw()
{
	// Draw all the (valid) entities
	for (unsigned int i = 0; i < mEnts.size(); i++)
		if (mEntStates[i] == 0)
			mEnts[i]->onDraw();
}

void EntityManager::onPhysics()
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
				_solveComplex(e1, e2);
			else
			if (e2->isBodyComplex())
				_solveComplex(e2, e1);
			
			// Otherwise, both objects are dynamic, so they'll contribute equally
			else
				_solveDynamic(e1, e2);
		}
	}
}

bool EntityManager::doRaycast(const Ray& ray, RaycastInfo* info)
{
	// Prepare hit info
	bool didHit = false;
	RaycastInfo bestInfo;
	
	// Go through all the (valid) entities
	for (unsigned int i = 0; i < mEnts.size(); i++)
	{
		// Skip invalid entities
		if (mEntStates[i] != 0)
			continue;
			
		// Get the entity
		Entity* e = mEnts[i];
			
		// Perform the raycast
		RaycastInfo tempInfo;
		if (e->isBodyCircle() && 
		      _raycastCircle(e, ray, &tempInfo) ||
		    e->isBodyComplex() && 
		      e->doComplexBodyRaycast(ray, &tempInfo))
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

void EntityManager::_onStateTick()
{
	// Update the validity states of the entity
	for (unsigned int i = 0; i < mEntStates.size(); i++) 
	{
		// state < 0 means it was just added, so make it valid
		if (mEntStates[i] < 0)
			mEntStates[i] = 0;
		else
		
		// state > 0 means it was just removed, so delete it
		if (mEntStates[i] > 1) {
			delete mEnts[i];
			mEnts.erase(mEnts.begin() + i);
			mEntStates.erase(mEntStates.begin() + i);
			i--;
			continue;
		}
	}
}

void EntityManager::_solveComplex(Entity* c, Entity* d)
{
	// Let the entity handle it
	c->fixComplexBodyCollision(d);
}

void EntityManager::_solveDynamic(Entity* d1, Entity* d2)
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

bool EntityManager::_raycastCircle(Entity* e, const Ray& r, RaycastInfo* i)
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
		i->point    =  Vec2(dist * rayDirCos, dist * rayDirSin) + r.position;
		i->normal   = (Vec2(dist, 0.0f) - tPos).normalized();
		i->normal   =  Vec2(i->normal.x * rayDirCos - i->normal.y * rayDirSin,
			                  i->normal.x * rayDirSin + i->normal.y * rayDirCos);
	}
	return true;
}
