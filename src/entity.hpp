#pragma once

#include "vec2.hpp"

class Entity {
public:
	Entity();
	virtual ~Entity();

	virtual void onTick();
	virtual void onDraw();

	void move(const Vec2& i);
	void setPos(const Vec2& p);
	Vec2 getPos() const;
	
	void  setRadius(const float r);
	float getRadius() const;

private:
	Vec2  mPos;
	float mRadius;
};
