#include "arrow.hpp"
#include "commongl.hpp"
#include <cmath>

Arrow::Arrow()
{
	mStuck = false;
	mSpeed = 14.0f;
	mFadeTimer.setGoal(1.0f);
	setType("arrow");
}

Arrow::~Arrow()
{
}

void Arrow::onInit()
{
	getTickGod()->addTo("weapons", this);
	getDrawGod()->addTo("weapons", this);
}

void Arrow::onKill()
{
	getTickGod()->removeFrom("weapons", this);
	getDrawGod()->removeFrom("weapons", this);
}

void Arrow::onTick(float dt)
{
	// If we are stuck, fade
	mFadeTimer.addTime(dt);

	// If we are moving, raycast in front of us
	if (!mStuck) 
	{
		// Get the ray info
		float moveDist  = getSpeed() * dt;
		float rayLength = getLength() + moveDist;
		Raycast ray(getPosition(), getDirection());
		
		// Do the raycast
		if (getGod()->doRaycast(ray, rayLength))
		{
			// Get the gameobject
			GameObject* go = ray.hitBody->getBodyGo();
			if (go != 0) 
			{
				// Hit a tile
				if (go->getType() == "tiles") {
					mStuck = true;
					setPosition(ray.hitPoint - getDirection() * getLength());
				} else
				
				// Hit an enemy
				if (go->getType() == "enemy") {
					go->onMessage("hit by arrow", this);
					getGod()->remove(this);
					return;
				}
			}
		}
	}
	
	// If we aren't stuck, move us forward
	if (!mStuck)
		translate(getDirection() * getSpeed() * dt);
	
	// Kill ourself
	if (getAlpha() <= 0.0f)
		getGod()->remove(this);
}

void Arrow::onDraw()
{
	CommonGL::setColor(Color(0.3f, 0.4f, 0.3f, getAlpha()));
	CommonGL::push();
	CommonGL::translate(getPosition());
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
	if (mFadeTimer.reachedGoal())
		return 0.0f;
	return mFadeTimer.timeUntilGoal() / mFadeTimer.getGoal();
}
