#include "entity.hpp"
#include "commongl.hpp"

Entity::Entity()
{
	mRot = 0.0f;
	mRadius = 0.5f;
	mIsSolid = true;
	mIsValid = true;
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

void Entity::onMessage(const std::string& s, void* d)
{
	// Do nothing.
}

void Entity::move(const Vec2& i)
{
	mPos += i;
}

void Entity::setPos(const Vec2& p)
{
	mPos = p;
}

Vec2 Entity::getPos() const
{
	return mPos;
}

void Entity::setRot(const float rad)
{
	mRot = rad;
}

float Entity::getRot() const
{
	return mRot;
}

float Entity::getRotAsDeg() const
{
	return mRot * 180.0f / 3.141592654f;
}

void Entity::setRadius(const float r)
{
	mRadius = r;
}

float Entity::getRadius() const
{
	return mRadius;
}

void Entity::setSolid(const bool s)
{
	mIsSolid = s;
}

bool Entity::isSolid() const
{
	return mIsSolid;
}

void Entity::invalidate()
{
	mIsValid = false;
}

bool Entity::isValid() const
{
	return mIsValid;
}
