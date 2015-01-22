#include "vec2.hpp"
#include <cmath>

Vec2::Vec2()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

Vec2::Vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vec2::Vec2(const Vec2& v)
{
	this->x = v.x;
	this->y = v.y;
}

Vec2::~Vec2()
{
}

void Vec2::operator=(const Vec2& v)
{
	this->x = v.x;
	this->y = v.y;
}

bool Vec2::operator==(const Vec2& v) const
{
	return (this->x == v.x && 
	        this->y == v.y);
}

bool Vec2::operator!=(const Vec2& v) const
{
	return (this->x != v.x ||
	        this->y != v.y);
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
	return Vec2(this->x + v.x, this->y + v.y);
}

Vec2 Vec2::operator-(const Vec2& v) const
{
	return Vec2(this->x - v.x, this->y - v.y);
}

Vec2 Vec2::operator*(const float f) const
{
	return Vec2(this->x * f, this->y * f);
}

Vec2 Vec2::operator/(const float f) const
{
	return Vec2(this->x / f, this->y / f);
}

float Vec2::getMag() const
{
	return std::sqrt(x * x + y * y);
}

float Vec2::getSqrMag() const
{
	return x * x + y * y;
}

Vec2 Vec2::getNormalized() const
{
	const float mag = getMag();
	if (mag == 0.0f)
		return Vec2();
	return (*this) / mag;
}

void Vec2::normalize()
{
	const float mag = getMag();
	if (mag == 0.0f)
		(*this) = Vec2();
	(*this) /= mag;
}


