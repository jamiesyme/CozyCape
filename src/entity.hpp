#pragma once

#include "vec2.hpp"
#include <string>
class Ray;
class RaycastInfo;
class EntityManager;

class Entity {
public:
	Entity();
	virtual ~Entity();
	
	virtual void onTick();
	virtual void onDraw();
	virtual void onMessage(const std::string& s, void* d);
	
	void setManager(EntityManager* mgr);
	EntityManager* getManager();
	
	void setName(const std::string& name);
	void setType(const std::string& type);
	std::string getName() const;
	std::string getType() const;
	
	void translate(const Vec2& v);
	void rotate(const float rad);
	
	void  setPosition(const Vec2& p);
	void  setRotation(const float rad);
	Vec2  getPosition()      const;
	float getRotation()      const;
	float getRotationAsDeg() const;
	
	void  setDepth(const float d);
	float getDepth() const;
	
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
	
	virtual bool fixComplexBodyCollision(Entity* e);
	virtual bool  doComplexBodyRaycast(const Ray& ray, RaycastInfo* info);

private:
	EntityManager* mMgr;
	std::string mName;
	std::string mType;
	Vec2  mPos;
	float mRot;
	float mDepth;
	int   mBodyType; // 1 = circle, 2 = rect, 3 = complex, ? = empty
	float mRadius;
	Vec2  mRect;
};
