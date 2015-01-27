#include "camera.hpp"
#include "commongl.hpp"
#include "window.hpp"
#include <cmath>

Camera::Camera(const Vec2& size)
{
	mFollow = 0;
	setSize(size);
}

Camera::~Camera()
{
}

void Camera::onDraw()
{
	// Don't draw the camera.
}

void Camera::setSize(const Vec2& s)
{
	mSize = s;
}

Vec2 Camera::getSize() const
{
	return mSize;
}

Vec2 Camera::getWorldPos(int pixelX, int pixelY) const
{
	Vec2 pos = getPos() - getSize() / 2.0f;
	pos.x += (float)pixelX / (float)Window::getWidth()  * getSize().x;
	pos.y += (float)pixelY / (float)Window::getHeight() * getSize().y;
	return pos;
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
	CommonGL::setOrtho(Vec2(0.0f, mSize.x),
	                   Vec2(mSize.y, 0.0f),
	                   Vec2(-1.0f, 1.0f));
	CommonGL::translate(-(getPos() - mSize / 2));
}

void Camera::follow(Entity* e)
{
	mFollow = e;
}
