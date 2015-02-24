#include "camera.hpp"
#include "commongl.hpp"
#include "window.hpp"

Camera::Camera(const std::string& name)
{
	mName = name;
}

Camera::~Camera()
{
}

std::string Camera::getName() const
{
	return mName;
}

void Camera::onBind()
{
	CommonGL::setOrtho(Vec2(mBl.x, mTr.x), 
	                   Vec2(mTr.y, mBl.y), 
	                   Vec2(-1.0f, 1.0f));
}

void Camera::set(const Vec2& bl, const Vec2& tr)
{
	mBl = bl;
	mTr = tr;
}

void Camera::center(const Vec2& on)
{
	const Vec2 size = getSize();
	mBl = on - size / 2.0f;
	mTr = on + size / 2.0f;
}

Vec2 Camera::getBottomLeft() const
{
	return mBl;
}

Vec2 Camera::getBottomRight() const
{
	return Vec2(mTr.x, mBl.y);
}

Vec2 Camera::getTopLeft() const
{
	return Vec2(mBl.x, mTr.y);
}

Vec2 Camera::getTopRight() const
{
	return mTr;
}

Vec2 Camera::getCenter() const
{
	return mBl + getSize() / 2.0f;
}

Vec2 Camera::getSize() const
{
	return Vec2(mTr.x - mBl.x, mTr.y - mBl.y);
}

Vec2 Camera::getWorldPos(const int pixelX, const int pixelY)
{
	const int ww = Window::getWidth();
	const int wh = Window::getHeight();
	Vec2 size = getSize();
	Vec2 pos  = getBottomLeft();
	pos.x += (float)pixelX / (float)ww * size.x;
	pos.y += (float)pixelY / (float)wh * size.y;
	return pos;
}
