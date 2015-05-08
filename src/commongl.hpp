#pragma once

#include "vec2.hpp"
#include "color.hpp"

namespace CommonGL {
	void enableAlphaBlend();
	void enableDepthTest();
	void enableTextures();

	void setColor(Color c);
	void setBgColor(Color c);
	
	void setOrtho(const Vec2& x, const Vec2& y, const Vec2& z);
	
	void clearColor();
	void clearDepth();
	
	void push();
	void pop();
	void translate(const Vec2& t);
	void translateZ(const float z);
	void rotateZ(const float deg);
	
	void drawRect(const Vec2& v1, const Vec2& v2);
	void drawCircle(const Vec2& p, const float radius, const int segs);
}
