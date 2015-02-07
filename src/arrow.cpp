#include "arrow.hpp"
#include "game.hpp"
#include "raycast.hpp"
#include <GL/gl.h>

Arrow::Arrow()
{
	mTickTimer.set(0.0);
	mDirection = Vec2(1.0f, 0.0f);
	mDuration  = 1.0f;
	mAlpha     = 1.0f;
	setType("arrow");
}

Arrow::~Arrow()
{
}

void Arrow::onTick()
{
	const float dt = (float)mTickTimer.get();
	mTickTimer.set(0.0);	
	
	mAlpha -= dt / mDuration;
	if (mAlpha <= 0.0f)
		Game::removeEntity(this);
}

void Arrow::onDraw()
{
	glColor3f(1.0f, 1.0f - mAlpha, 1.0f - mAlpha);
	glBegin(GL_LINES);
	for (int i = 0; i < mTrail.size(); i += 2)
	{
		const Vec2 p1 = mTrail[i + 0];
		const Vec2 p2 = mTrail[i + 1];
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	}
	glEnd();
}

void Arrow::setDirection(const Vec2& dir)
{
	if (dir.length2() == 0.0f)
		return;
	mDirection = dir.normalized();
}

void Arrow::setDuration(const float d)
{
	if (d <= 0.0f)
		return;
	mDuration = d;
}

void Arrow::shoot()
{
	// Clear the trail
	mTrail.clear();
	
	// Raycast
	Ray ray(getPosition(), mDirection);
	RaycastInfo info;
	if (Game::doRaycast(ray, &info))
	{
		mTrail.push_back(getPosition());
		mTrail.push_back(info.point);
	} else
	{
		mTrail.push_back(getPosition());
		mTrail.push_back(getPosition() + mDirection * 1000.0f);
	}
}
