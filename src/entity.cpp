#include "entity.hpp"
#include <GL/gl.h>
#include <cmath>

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
	const int kSegs = 12;
	const float kFullRad = 3.141592654f * 2.0f;
	glPushMatrix();
	glTranslatef(mPos.x, mPos.y, 0.0f);
	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < kSegs; i++)
	{
		const float rad1 = (float)(i + 0) / (float)kSegs * kFullRad;
		const float rad2 = (float)(i + 1) / (float)kSegs * kFullRad;
		const Vec2 p1 = Vec2(std::cos(rad1) * mRadius, 
												 std::sin(rad1) * mRadius);
		const Vec2 p2 = Vec2(std::cos(rad2) * mRadius, 
												 std::sin(rad2) * mRadius);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	}
	glEnd();
	glPopMatrix();
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
