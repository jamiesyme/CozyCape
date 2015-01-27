#include "vec2.hpp"
#include <cmath>

Vec2::Vec2()
{
	x = 0.0f;
	y = 0.0f;
}

Vec2::Vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vec2::Vec2(const Vec2& v)
{
	x = v.x;
	y = v.y;
}

Vec2::~Vec2()
{
}

void Vec2::operator=(const Vec2& v)
{
	x = v.x;
	y = v.y;
}

bool Vec2::operator==(const Vec2& v) const
{
	return (x == v.x && 
	        y == v.y);
}

bool Vec2::operator!=(const Vec2& v) const
{
	return (x != v.x ||
	        y != v.y);
}

float& Vec2::operator[](const int i)
{
	if (i == 0) return x;
	else        return y;
}

const float& Vec2::operator[](const int i) const
{
	if (i == 0) return x;
	else        return y;
}

void Vec2::operator+=(const Vec2& v)
{
	x += v.x;
	y += v.y;
}

void Vec2::operator-=(const Vec2& v)
{
	x -= v.x;
	y -= v.y;
}

void Vec2::operator*=(const float f)
{
	x *= f;
	y *= f;
}

void Vec2::operator/=(const float f)
{
	x /= f;
	y /= f;
}

Vec2 Vec2::operator-() const
{
	return Vec2(-x, -y);
}

Vec2 Vec2::operator+(const Vec2& v) const
{
	return Vec2(x + v.x, 
	            y + v.y);
}

Vec2 Vec2::operator-(const Vec2& v) const
{
	return Vec2(x - v.x, 
	            y - v.y);
}

Vec2 Vec2::operator*(const float f) const
{
	return Vec2(x * f, 
	            y * f);
}

Vec2 Vec2::operator/(const float f) const
{
	return Vec2(x / f, 
	            y / f);
}

Vec2 operator*(const float f, const Vec2& v)
{
	return Vec2(v.x * f,
	            v.y * f);
}

Vec2 operator/(const float f, const Vec2& v)
{
	return Vec2(v.x / f,
	            v.y / f);
}

float Vec2::length() const
{
	return std::sqrt(x * x + y * y);
}

float Vec2::length2() const
{
	return x * x + y * y;
}

float Vec2::distance(const Vec2& v) const
{
	return (v - *this).length();
}

float Vec2::distance2(const Vec2& v) const
{
	return (v - *this).length2();
}

Vec2 Vec2::normalized() const
{
	const float len = length();
	if (len == 0.0f) 
		return Vec2();
	else
		return (*this) / len;
}

void Vec2::normalize()
{
	const float len = length();
	if (len == 0.0f)
		(*this) = Vec2();
	else
		(*this) /= len;
}

float Vec2::dot(const Vec2& v) const
{
	return x * v.x + y * v.y;
}
