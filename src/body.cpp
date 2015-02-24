#include "body.hpp"

Body::Body()
{
	mGo  = 0;
	mPos = Vec2(0.0f, 0.0f);
	mRot = 0.0f;
	setBodyEmpty();
}

Body::~Body()
{
}

void Body::setBodyGo(GameObject* go)
{
	mGo = go;
}

GameObject* Body::getBodyGo()
{
	return mGo;
}

void Body::translate(const Vec2& v)
{
	mPos += v;
}

void Body::rotate(const float rad)
{
	mRot += rad;
}

void Body::setPosition(const Vec2& p)
{
	mPos = p;
}

void Body::setRotation(const float rad)
{
	mRot = rad;
}

Vec2 Body::getPosition() const
{
	return mPos;
}

float Body::getRotation() const
{
	return mRot;
}

float Body::getRotationAsDeg() const
{
	return mRot * 180.0f / 3.141592654f;
}

void Body::setBodyEmpty()
{
	mBodyType = 0;
}

void Body::setBodyCircle(const float radius)
{
	mBodyType = 1;
	mRadius = radius;
}

void Body::setBodyRect(const Vec2& size)
{
	mBodyType = 2;
	mRect = size;
}

void Body::setBodyComplex()
{
	mBodyType = 3;
}

bool Body::isBodyEmpty() const
{
	return (mBodyType < 1 || mBodyType > 3);
}

bool Body::isBodyCircle() const
{
	return (mBodyType == 1);
}

bool Body::isBodyRect() const
{
	return (mBodyType == 2);
}

bool Body::isBodyComplex() const
{
	return (mBodyType == 3);
}

float Body::getBodyRadius() const
{
	return (isBodyCircle() ? mRadius : 0.0f);
}

Vec2 Body::getBodyRect() const
{
	return (isBodyRect() ? mRect : Vec2());
}

bool Body::fixBodyCollision(Body* b)
{
	return false;
}

bool Body::doRaycast(Raycast& ray)
{
	return false;
}
