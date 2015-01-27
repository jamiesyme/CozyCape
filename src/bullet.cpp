#include "bullet.hpp"
#include "map.hpp"
#include <GL/gl.h>

Bullet::Bullet()
{
	mTickTimer.set(0.0);
	mDirection = Vec2(1.0f, 0.0f);
	mBounceMax = 10;
	mDuration = 1.0f;
	mAlpha = 1.0f;
	setSolid(false);
}

Bullet::~Bullet()
{
}

void Bullet::onTick()
{
	const float dt = (float)mTickTimer.get();
	mTickTimer.set(0.0);	
	
	mAlpha -= dt / mDuration;
	if (mAlpha <= 0.0f)
		invalidate();
}

void Bullet::onDraw()
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

void Bullet::setDirection(const Vec2& dir)
{
	if (dir.length2() == 0.0f)
		return;
	mDirection = dir.normalized();
}

void Bullet::setBounceMax(const int i)
{
	if (i < 0)
		return;
	mBounceMax = i;
}

void Bullet::setDuration(const float d)
{
	if (d <= 0.0f)
		return;
	mDuration = d;
}

void Bullet::fire()
{
	mTrail.clear();
	Vec2 pos = getPos();
	Vec2 dir = mDirection;
	
	for (int i = 0; i < mBounceMax + 1; i++) 
	{
		Map::Raycast ray;
		ray.pos = pos;
		ray.dir = dir;
		Map::RaycastHitTile hitInfo;
		if (Map::tileRaycast(ray, &hitInfo)) {
			mTrail.push_back(ray.pos);
			mTrail.push_back(hitInfo.point);
			pos = hitInfo.point + hitInfo.normal * 0.001f;
			dir = dir - 2 * dir.dot(hitInfo.normal) * hitInfo.normal;
			continue;
		} else {
			mTrail.push_back(ray.pos);
			mTrail.push_back(ray.pos + ray.dir * 1000.0f);
			break;
		}
	}
}
