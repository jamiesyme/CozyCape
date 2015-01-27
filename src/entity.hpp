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
	
	void setSolid(const bool s);
	bool  isSolid() const;
	
	void invalidate();
	bool isValid() const;

private:
	bool  mIsValid;
	bool  mIsSolid;
	Vec2  mPos;
	float mRadius;
};
