#pragma once

#include <string>
#include "vec2.hpp"

class Camera {
public:
	Camera(const std::string& name);
	~Camera();
	
	std::string getName() const;
	
	void onBind();
	
	void set(const Vec2& bl, const Vec2& tr);
	void center(const Vec2& on);
	Vec2 getBottomLeft() const;
	Vec2 getBottomRight() const;
	Vec2 getTopLeft() const;
	Vec2 getTopRight() const;
	Vec2 getCenter() const;
	Vec2 getSize() const;
	
	Vec2 getWorldPos(const int pixelX, const int pixelY);

private:
	std::string mName;
	Vec2 mBl;
	Vec2 mTr;
};
