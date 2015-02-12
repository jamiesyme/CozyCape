#include "arrow.hpp"
#include "entitymanager.hpp"
#include "raycast.hpp"
#include "commongl.hpp"
#include <cmath>

Arrow::Arrow()
{
	mTickTimer.set(0.0);
	mSpeed   = 14.0f;
	mHitTime = -1.0;
	setType("arrow");
	setDepth(-0.55f);
}

Arrow::~Arrow()
{
}

void Arrow::onTick()
{
	const float dt = (float)mTickTimer.get();
	mTickTimer.set(0.0);	
	
	// Calculate movement
	Vec2 movement = getDirection() * getSpeed() * dt;
	
	// If we're moving, raycast
	if (mHitTime < 0) {
		float length = getLength() + movement.length();
		Ray ray      = Ray(getPosition(), getDirection());
		RaycastInfo info;
		if (getManager()->doRaycast(ray, &info) &&
		    info.distance <= length) 
		{
			if (info.entity->getType() == "tiles") {
				movement = info.point - (getPosition() + getDirection() * getLength());
				mHitTime = Clock::getTime();
			} else
			if (info.entity->getType() == "enemy") {
				info.entity->onMessage("hit by arrow", this);
				getManager()->remove(this);
				return;
			}
		}
	} else
		movement = Vec2();
		
	// Move
	translate(movement);
	
	// Kill ourself
	if (getAlpha() <= 0.0f)
		getManager()->remove(this);
}

void Arrow::onDraw()
{
	CommonGL::setColor(Color(0.3f, 0.4f, 0.3f, getAlpha()));
	CommonGL::push();
	CommonGL::translate(getPosition());
	CommonGL::translateZ(getDepth());
	CommonGL::rotateZ(getRotationAsDeg());
	CommonGL::drawRect(Vec2(0.0f, -0.05f), Vec2(getLength(), 0.05f));
	CommonGL::pop();
}

void Arrow::setDirection(const Vec2& dir)
{
	setRotation(std::atan2(dir.y, dir.x));
}

void Arrow::setSpeed(const float speed)
{
	mSpeed = speed;
}

Vec2 Arrow::getDirection() const
{
	const float rad = getRotation();
	return Vec2(std::cos(rad), std::sin(rad));
}

float Arrow::getSpeed() const
{
	return mSpeed;
}

float Arrow::getLength() const
{
	return 0.6f;
}

float Arrow::getAlpha() const
{
	if (mHitTime < 0.0)
		return 1.0f;
	const double time = (Clock::getTime() - mHitTime);
	return (float)(1.0f - time);
}
