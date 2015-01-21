#pragma once

class Color {
public:
	typedef unsigned char byte;
	byte r;
	byte g;
	byte b;
	byte a;
	
	Color();
	Color(byte r, byte g, byte b);
	Color(byte r, byte g, byte b, byte a);
	Color(float r, float g, float b);
	Color(float r, float g, float b, float a);
	Color(const Color& c);
	~Color();
	void operator=(const Color& c);
	bool operator==(const Color& c) const;
	bool operator!=(const Color& c) const;
	byte& operator[](const int i);
	const byte& operator[](const int i) const;
	
	static Color Red;
	static Color Green;
	static Color Blue;
	static Color Black;
	static Color White;
	static Color Grey;
	static Color Clear;
};
