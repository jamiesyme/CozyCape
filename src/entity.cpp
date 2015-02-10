#include "entity.hpp"

Entity::Entity()
{
	mMgr      = 0;
	mName     = std::string();
	mType     = std::string();
	mPos      = Vec2();
	mRot      = 0.0f;
	mBodyType = 0;     // Empty
	mRadius   = 0.0f;
	mRect     = Vec2();
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
	// Draw nothing.
}

void Entity::onMessage(const std::string& s, void* d)
{
	// Do nothing.
}

void Entity::setManager(EntityManager* mgr)
{
	mMgr = mgr;
}

EntityManager* Entity::getManager()
{
	return mMgr;
}	

void Entity::setName(const std::string& name)
{
	mName = name;
}

void Entity::setType(const std::string& type)
{
	mType = type;
}

std::string Entity::getName() const
{
	return mName;
}

std::string Entity::getType() const
{
	return mType;
}

void Entity::translate(const Vec2& v)
{
	mPos += v;
}

void Entity::rotate(const float rad)
{
	mRot += rad;
}

void Entity::setPosition(const Vec2& p)
{
	mPos = p;
}

void Entity::setRotation(const float rad)
{
	mRot = rad;
}

Vec2 Entity::getPosition() const
{
	return mPos;
}

float Entity::getRotation() const
{
	return mRot;
}

float Entity::getRotationAsDeg() const
{
	return mRot * 180.0f / 3.141592654f;
}

void Entity::setBodyEmpty()
{
	mBodyType = 0;
}

void Entity::setBodyCircle(const float radius)
{
	mBodyType = 1;
	mRadius = radius;
}

void Entity::setBodyRect(const Vec2& size)
{
	mBodyType = 2;
	mRect = size;
}

void Entity::setBodyComplex()
{
	mBodyType = 3;
}

bool Entity::isBodyEmpty() const
{
	return (mBodyType < 1 || mBodyType > 3);
}

bool Entity::isBodyCircle() const
{
	return (mBodyType == 1);
}

bool Entity::isBodyRect() const
{
	return (mBodyType == 2);
}

bool Entity::isBodyComplex() const
{
	return (mBodyType == 3);
}

float Entity::getBodyRadius() const
{
	return (isBodyCircle() ? mRadius : 0.0f);
}

Vec2 Entity::getBodyRect() const
{
	return (isBodyRect() ? mRect : Vec2());
}

bool Entity::fixComplexBodyCollision(Entity* e)
{
	return false;
}

bool Entity::doComplexBodyRaycast(const Ray& ray, RaycastInfo* info)
{
	return false;
}
