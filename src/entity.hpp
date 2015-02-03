#pragma once

#include "vec2.hpp"
#include <string>

class Entity {
public:
	Entity();
	virtual ~Entity();

	virtual void onTick();
	virtual void onDraw();
	virtual void onMessage(const std::string& s, void* d = 0);

	void move(const Vec2& i);
	void setPos(const Vec2& p);
	Vec2 getPos() const;
	
	void  setRot(const float rad);
	float getRot() const;
	float getRotAsDeg() const;
	
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
	float mRot;
	float mRadius;
};
