#include "physicsgod.hpp"
#include <cmath>

PhysicsGod::PhysicsGod()
{
}

PhysicsGod::~PhysicsGod()
{
}

void PhysicsGod::onTick(float dt)
{
	// Check all bodies against all other bodies
	for (int i = 0; i < (int)mBodies.size() - 1; i++) 
	{
		// Skip empty bodies
		Body* b1 = mBodies[i];
		if (b1->isBodyEmpty())
			continue;
		
		// Loop against the unchecked bodies
		for (int j = i + 1; j < (int)mBodies.size(); j++) 
		{
			// Skip empty bodies
			Body* b2 = mBodies[j];
			if (b2->isBodyEmpty())
				continue;
			
			// If both bodies are complex, nothing can happen
			if (b1->isBodyComplex() && b2->isBodyComplex())
				continue;
				
			// If only one body is complex, it will control the outcome
			if (b1->isBodyComplex())
				_solveComplex(b1, b2);
			else
			if (b2->isBodyComplex())
				_solveComplex(b2, b1);
			
			// Otherwise, both bodies are dynamic, so they'll contribute equally
			else
				_solveDynamic(b1, b2);
		}
	}
}

bool PhysicsGod::doRaycast(Raycast& ray, const float maxDist)
{
	if (!doRaycast(ray))
		return false;
	if (ray.hitDistance > maxDist)
		return false;
	return true;
}

bool PhysicsGod::doRaycast(Raycast& ray)
{
	// Prepare hit info
	Raycast bestHit;
	
	// Go through all the bodies
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{
		// Get the body
		Body* b = mBodies[i];
			
		// Perform the raycast
		Raycast tempRay(ray.origin, ray.direction);
		if (b->isBodyCircle() && 
		      _doRaycastCircle(b, tempRay) ||
		    b->isBodyComplex() && 
		      b->doRaycast(tempRay))
		{
			// Fill in the missing info
			tempRay.hitBody     = b;
			tempRay.hitDistance = (tempRay.hitPoint - tempRay.origin).length();
			
			// Overwrite the best result
			if (bestHit.hitBody == 0 || 
			    bestHit.hitDistance > tempRay.hitDistance)
				bestHit = tempRay;
		}
	}
	
	// If we missed, quit
	if (bestHit.hitBody == 0)
		return false;
		
	// We hit something, save the info
	ray = bestHit;
	return true;
}

void PhysicsGod::add(Body* b)
{
	if (b != 0)
		mBodies.push_back(b);
}

void PhysicsGod::remove(Body* b)
{
	for (unsigned int i = 0; i < mBodies.size(); i++) {
		if (b == mBodies[i]) {
			mBodies.erase(mBodies.begin() + i);
			return;
		}
	}
}

void PhysicsGod::removeAll()
{
	mBodies.clear();
}

void PhysicsGod::_solveComplex(Body* complex, Body* other)
{
	// Let the custom logic commence!
	complex->fixBodyCollision(other);
}

void PhysicsGod::_solveDynamic(Body* b1, Body* b2)
{
	// NOTE: Only solves circle vs circle right now!
	if (b1->isBodyCircle() && b2->isBodyCircle()) 
	{
		const float r1  = b1->getBodyRadius();
		const float r2  = b2->getBodyRadius();
		const Vec2 diff = b2->getPosition() - b1->getPosition();
		if (diff.length() < r1 + r2)
		{
			const Vec2 corr = diff.normalized() * (diff.length() - (r1 + r2));
			b1->translate( corr / 2);
			b2->translate(-corr / 2);
		}
	}
}

bool PhysicsGod::_doRaycastCircle(Body* b, Raycast& ray)
{
	// Get some information
	const float rayDirRadians = std::atan2(ray.direction.y, ray.direction.x);
	const float rayDirCos     = std::cos( rayDirRadians);
	const float rayDirSin     = std::sin( rayDirRadians);
	const float rayDirCosNeg  = std::cos(-rayDirRadians);
	const float rayDirSinNeg  = std::sin(-rayDirRadians);
	const float radius = b->getBodyRadius();

	// Get the transformed pos
	Vec2 tPos = b->getPosition() - ray.origin;
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
	Vec2 normal = (Vec2(dist, 0.0f) - tPos).normalized();
	normal = Vec2(ray.hitNormal.x * rayDirCos - ray.hitNormal.y * rayDirSin,
		            ray.hitNormal.x * rayDirSin + ray.hitNormal.y * rayDirCos);
	ray.hitPoint  = Vec2(dist * rayDirCos, dist * rayDirSin) + ray.origin;
	ray.hitNormal = normal;
	return true;
}
