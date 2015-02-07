#pragma once

#include "vec2.hpp"
class Entity;

class RaycastInfo {
public:
	Entity* entity;
	float   distance;
	Vec2    point;
	Vec2    normal;
	
	RaycastInfo();
	~RaycastInfo();
};
