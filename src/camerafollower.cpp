#include "camerafollower.hpp"
#include "camera.hpp"
#include "body.hpp"
#include "tickgod.hpp"

CameraFollower::CameraFollower(Camera* cam)
{
	mCam    = cam;
	mBody   = 0;
	mRadius = 0.0f;
}

CameraFollower::~CameraFollower()
{
}

void CameraFollower::onInit()
{
	getTickGod()->addTo("post", this);
}

void CameraFollower::onKill()
{
	getTickGod()->removeFrom("post", this);
}

void CameraFollower::onTick(float dt)
{
	if (mCam == 0 || mBody == 0)
		return;
	Vec2 cPos = mCam->getCenter();
	Vec2 fPos = mBody->getPosition();
	Vec2 diff = fPos - cPos;
	if (diff.length() <= mRadius)
		return;
	Vec2 move = diff.normalized() * (diff.length() - mRadius);
	mCam->center(cPos + move);
}

void CameraFollower::follow(Body* b)
{
	mBody = b;
}

void CameraFollower::setRadius(const float radius)
{
	if (radius < 0.0f)
		mRadius = 0.0f;
	else
		mRadius = radius;
}
