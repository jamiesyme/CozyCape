#pragma once

#include "vec2.hpp"
class Image;

class Texture {
public:
	Texture();
	~Texture();
	
	void load(Image& img);
	
	void onDraw(const Vec2& pos) const;
	void onDraw(const Vec2& pos, const Vec2& size) const;
	void bind()   const;
	void unbind() const;
	
	Vec2 getSize()  const;
	int getWidth()  const;
	int getHeight() const;

private:
	Vec2 mSize;
	unsigned int mId;
};
