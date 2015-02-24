#pragma once

#include "vec2.hpp"
#include "raycast.hpp"
class GameObject;

class Body {
public:
	Body();
	virtual ~Body();
	
	void setBodyGo(GameObject* go);
	GameObject* getBodyGo();
	
	void translate(const Vec2& v);
	void rotate(const float rad);
	
	void  setPosition(const Vec2& p);
	void  setRotation(const float rad);
	Vec2  getPosition()      const;
	float getRotation()      const;
	float getRotationAsDeg() const;
	
	void setBodyEmpty();
	void setBodyCircle(const float radius);
	void setBodyRect(const Vec2& size);
	void setBodyComplex();
	
	bool isBodyEmpty()   const;
	bool isBodyCircle()  const;
	bool isBodyRect()    const;
	bool isBodyComplex() const;
	
	float getBodyRadius() const;
	Vec2  getBodyRect()   const;
	
	virtual bool fixBodyCollision(Body* b);
	virtual bool doRaycast(Raycast& ray);

private:
	GameObject* mGo;
	Vec2  mPos;
	float mRot;
	int   mBodyType; // 1 = circle, 2 = rect, 3 = complex, ? = empty
	float mRadius;
	Vec2  mRect;
};
