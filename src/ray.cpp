#include "ray.hpp"

Ray::Ray()
{
}

Ray::Ray(const Vec2& pos, const Vec2& dir)
{
	position = pos;
	direction = dir;
}

Ray::~Ray()
{
}
