#pragma once

#include "vec2.hpp"

class Ray {
public:
	Vec2 position;
	Vec2 direction;
	
	Ray();
	Ray(const Vec2& pos, const Vec2& dir);
	~Ray();
};
