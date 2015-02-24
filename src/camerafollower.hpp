#pragma once

#include "gameobject.hpp"
#include "tickable.hpp"
class Camera;
class Body;

class CameraFollower : public GameObject, 
                       public Tickable {
public:
	CameraFollower(Camera* cam);
	~CameraFollower();
	
	void onInit();
	void onKill();
	void onTick(float dt);
	
	void follow(Body* b);
	void setRadius(const float r);

private:
	Camera* mCam;
	Body*   mBody;
	float   mRadius;
};
