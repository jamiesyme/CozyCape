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
	Vec2 dir  = mDirection;
	Vec2 pos1 = getPos();
	Vec2 pos2 = Vec2();
	Vec2 norm = Vec2();
	
	for (int i = 0; i < mBounceMax + 1; i++) 
	{
		// Raycasting
		Map::Raycast ray;
		ray.pos = pos1;
		ray.dir = dir;
		Map::RaycastHitTile   hitInfo1;
		Map::RaycastHitEntity hitInfo2;
		hitInfo1.dist = -1.0f;
		hitInfo2.dist = -1.0f;
		Map::tileRaycast(ray, &hitInfo1);
		Map::entityRaycast(ray, &hitInfo2);
		if (hitInfo2.dist <= hitInfo1.dist &&
		    hitInfo2.dist >= 0.0f) {
		  pos2 = hitInfo2.point;
		  norm = hitInfo2.normal;
		} else
		if (hitInfo1.dist >= 0.0f) {
			pos2 = hitInfo1.point;
			norm = hitInfo1.normal;
		} else {
			pos2 = pos1 + dir * 1000.0f;
			norm = Vec2();
		}
		
		// Hit the entity
		if (hitInfo2.dist <= hitInfo1.dist &&
				hitInfo2.dist >= 0.0f) {
			float damage = 100.0f;
			hitInfo2.ent->onMessage("hit", &damage);	
		}
		
		// Make the trail
		mTrail.push_back(pos1);
		mTrail.push_back(pos2);
		pos1 = pos2 + norm * 0.01f;
		if (norm.length2() != 0.0f)
			dir = dir - 2 * dir.dot(norm) * norm;
		else
			break;
	}
}
