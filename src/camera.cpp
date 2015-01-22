#include "camera.hpp"
#include "commongl.hpp"
#include <cmath>

Camera::Camera(float w, float h)
{
	mWidth = 0.0f;
	mHeight = 0.0f;
	mFollow = 0;
	setSize(w, h);
}

Camera::~Camera()
{
}

void Camera::onDraw()
{
	// Don't draw the camera.
}

void Camera::setSize(float w, float h)
{
	mWidth = w;
	mHeight = h;
}

float Camera::getWidth() const
{
	return mWidth;
}

float Camera::getHeight() const
{
	return mHeight;
}

void Camera::bind()
{
	// Follow our entity
	if (mFollow != 0) 
	{
		const float dist = getPos().distance(mFollow->getPos());
		if (dist > getRadius()) 
		{
			const float diff = dist - getRadius();
			move((mFollow->getPos() - getPos()).normalized() * diff);
		}
	}

	// Apply the camera settings to OpenGL
	CommonGL::setOrtho(Vec2(0.0f, mWidth),
	                   Vec2(mHeight, 0.0f),
	                   Vec2(-1.0f, 1.0f));
	CommonGL::translate(-(getPos() - Vec2(mWidth, mHeight) / 2));
}

void Camera::follow(Entity* e)
{
	mFollow = e;
}
