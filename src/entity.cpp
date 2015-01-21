#include "entity.hpp"
#include "commongl.hpp"

Entity::Entity()
{
	mRadius = 0.5f;
}

Entity::~Entity()
{
}

void Entity::onTick()
{
	// Do nothing.
}

void Entity::onDraw()
{
	// Draw the entity.
	CommonGL::setColor(Color::Black);
	CommonGL::drawCircle(mPos, mRadius, 24);
	CommonGL::setColor(Color::White);
	CommonGL::drawCircle(mPos, mRadius * 0.8f, 24);
}

void Entity::setPos(const Vec2& p)
{
	mPos = p;
}

Vec2 Entity::getPos() const
{
	return mPos;
}

void Entity::setRadius(const float r)
{
	mRadius = r;
}

float Entity::getRadius() const
{
	return mRadius;
}
