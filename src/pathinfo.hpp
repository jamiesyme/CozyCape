#pragma once

#include "vec2.hpp"
#include <vector>

class PathInfo {
public:
	PathInfo();
	~PathInfo();
	
	void append(const Vec2& n); // Add to end
	void push(const Vec2& n);   // Add to front
	void clear();
	
	Vec2 get(int i) const;
	int  getCount() const;

private:
	std::vector<Vec2> mNodes;
};
