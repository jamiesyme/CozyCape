#pragma once

#include "entity.hpp"

class Camera : public Entity {
public:
	Camera(float w, float h);
	~Camera();
	
	void onDraw();
	
	void  setSize(float w, float h);
	float getWidth() const;
	float getHeight() const;
	
	void bind();
	
	void follow(Entity* e);	
	
private:
	float mWidth;
	float mHeight;
	Entity* mFollow;
};
