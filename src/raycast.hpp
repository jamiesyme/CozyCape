#pragma once

#include "vec2.hpp"
class Body;

class Raycast {
public:
	Vec2   origin;
	Vec2   direction;
	Body*  hitBody;
	float  hitDistance;
	Vec2   hitPoint;
	Vec2   hitNormal;
	
	Raycast();
	Raycast(const Vec2& ori, const Vec2& dir);
	~Raycast();
};
