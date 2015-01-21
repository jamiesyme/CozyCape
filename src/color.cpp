#include "color.hpp"

Color Color::Red(1.0f, 0.0f, 0.0f);
Color Color::Green(0.0f, 1.0f, 0.0f);
Color Color::Blue(0.0f, 0.0f, 1.0f);
Color Color::Black(0.0f, 0.0f, 0.0f);
Color Color::White(1.0f, 1.0f, 1.0f);
Color Color::Grey(0.5f, 0.5f, 0.5f);
Color Color::Clear(1.0f, 1.0f, 1.0f, 0.0f);

Color::Color()
{
	this->r = 255;
	this->g = 255;
	this->b = 255;
	this->a = 255;
}

Color::Color(byte r, byte g, byte b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 255;
}

Color::Color(byte r, byte g, byte b, byte a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(float r, float g, float b)
{
	this->r = (byte)(r * 255.0f);
	this->g = (byte)(g * 255.0f);
	this->b = (byte)(b * 255.0f);
	this->a = 255;
}

Color::Color(float r, float g, float b, float a)
{
	this->r = (byte)(r * 255.0f);
	this->g = (byte)(g * 255.0f);
	this->b = (byte)(b * 255.0f);
	this->a = (byte)(a * 255.0f);
}

Color::Color(const Color& c)
{
	this->r = c.r;
	this->g = c.g;
	this->b = c.b;
	this->a = c.a;
}

Color::~Color()
{
}

void Color::operator=(const Color& c)
{
	this->r = c.r;
	this->g = c.g;
	this->b = c.b;
	this->a = c.a;
}

bool Color::operator==(const Color& c) const
{
	return (this->r == c.r &&
	         this->g == c.g &&
	         this->b == c.b &&
	         this->a == c.a);
}

bool Color::operator!=(const Color& c) const
{
	return (this->r != c.r ||
	         this->g != c.g ||
	         this->b != c.b ||
	         this->a != c.a);
}

Color::byte& Color::operator[](const int i)
{
	if (i == 0) return r;
	if (i == 1) return g;
	if (i == 2) return b;
	else        return a;
}

const Color::byte& Color::operator[](const int i) const
{
	if (i == 0) return r;
	if (i == 1) return g;
	if (i == 2) return b;
	else        return a;
}
