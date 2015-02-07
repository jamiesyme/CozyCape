#pragma once

#include <string>

class Vec2 {
public:
	float x;
	float y;

	Vec2();
	Vec2(float x, float y);
	Vec2(const Vec2& v);
	~Vec2();
	void operator=(const Vec2& v);
	bool operator==(const Vec2& v) const;
	bool operator!=(const Vec2& v) const;
	float& operator[](const int i);
	const float& operator[](const int i) const;
	
	void operator+=(const Vec2& v);
	void operator-=(const Vec2& v);
	void operator*=(const float f);
	void operator/=(const float f);
	
	Vec2 operator-() const;
	Vec2 operator+(const Vec2& v) const;
	Vec2 operator-(const Vec2& v) const;
	Vec2 operator*(const float f) const;
	Vec2 operator/(const float f) const;
	friend Vec2 operator*(const float f, const Vec2& v);
	friend Vec2 operator/(const float f, const Vec2& v);
	
	float distance(const Vec2& v) const;
	float distance2(const Vec2& v) const;
	float length() const;
	float length2() const;
	
	Vec2  normalized() const;
	void  normalize();
	
	float dot(const Vec2& v) const;
	
	std::string toString() const;
};
