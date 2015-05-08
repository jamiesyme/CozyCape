#include "sword.hpp"
#include "body.hpp"
#include "commongl.hpp"
#include <cmath>

Sword::Sword(Body* user)
{
	mUser = user;
	mTimer.setGoal(0.2f);
	mTimer.addTime(mTimer.getGoal()); // Don't swing right away
	mBaseRot = 90.0f;
	mCurRot = mBaseRot;
}

Sword::~Sword()
{
}

void Sword::onInit()
{
	getTickGod()->addTo("weapons", this);
	getDrawGod()->addTo("weapons", this);
}

void Sword::onKill()
{
	getTickGod()->removeFrom("weapons", this);
	getDrawGod()->removeFrom("weapons", this);
}

void Sword::onTick(float dt)
{
	// Increase the timer
	mTimer.addTime(dt);
	
	// Set the position and rotation
	float percentGoal = mTimer.getTime() / mTimer.getGoal();
	float rotation = mBaseRot - mBaseRot * 2.0f * percentGoal;
	setRotation(mUser->getRotation() + rotation / 180.0f * 3.141592654f);
	setPosition(mUser->getPosition());
	
	// Raycast the sword
	if (!mTimer.reachedGoal()) 
	{
		// Get the ray info
		Raycast ray(mUser->getPosition(), getDirection());
		
		// Do the raycast
		if (getGod()->doRaycast(ray, getLength()))
		{
			// Get the gameobject
			GameObject* go = ray.hitBody->getBodyGo();
			if (go != 0)
			{
				// Hit an enemy
				if (go->getType() == "enemy")
					go->onMessage("hit by sword", this);
			}
		}
	}
}

void Sword::onDraw()
{
	if (mTimer.reachedGoal())
		return;
	CommonGL::setColor(Color(0.5f, 0.3f, 0.3f));
	CommonGL::push();
	CommonGL::translate(getPosition());
	CommonGL::rotateZ(getRotationAsDeg());
	CommonGL::drawRect(Vec2(0.0f, -0.05f), Vec2(getLength(), 0.05f));
	CommonGL::pop();
}

void Sword::ready()
{
}

void Sword::attack()
{
	// Reset the swing
	mCurRot = mBaseRot;
	mTimer.reset();
}

float Sword::getLength() const
{
	return 1.0f;
}

Vec2 Sword::getDirection() const
{
	return Vec2(std::cos(getRotation()), std::sin(getRotation()));
}
