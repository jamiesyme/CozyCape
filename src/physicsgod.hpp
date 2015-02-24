#pragma once

#include <vector>
#include "tickable.hpp"
#include "body.hpp"
#include "raycast.hpp"

class PhysicsGod : public Tickable {
public:
	PhysicsGod();
	~PhysicsGod();
	
	void onTick(float dt);
	bool doRaycast(Raycast& ray);
	bool doRaycast(Raycast& ray, const float maxDist);
	
	void    add(Body* b);
	void remove(Body* b);
	void removeAll();

private:
	std::vector<Body*> mBodies;
	
	void _solveComplex(Body* complex, Body* other);
	void _solveDynamic(Body* b1, Body* b2);
	bool _doRaycastCircle(Body* b, Raycast& ray);
};
