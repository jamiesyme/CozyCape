#include "raycast.hpp"

Raycast::Raycast()
{
	hitBody     = 0;
	hitDistance = 0.0f;
}

Raycast::Raycast(const Vec2& ori, const Vec2& dir)
{
	origin      = ori;
	direction   = dir;
	hitBody     = 0;
	hitDistance = 0.0f;
}

Raycast::~Raycast()
{
}
