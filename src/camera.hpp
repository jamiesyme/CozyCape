#pragma once

#include "entity.hpp"

class Camera : public Entity {
public:
	Camera(const Vec2& size);
	~Camera();
	
	void setSize(const Vec2& s);
	Vec2 getSize() const;
	
	Vec2 getWorldPos(int pixelX, int pixelY) const;
	
	void bind();
	void follow(Entity* e, float radius);
	
private:
	Vec2    mSize;
	Entity* mFollow;
	float   mFollowRadius;
};
