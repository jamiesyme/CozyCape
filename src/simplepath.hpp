#pragma once

#include "vec2.hpp"

class SimplePath {
public:
	SimplePath();
	SimplePath(const Vec2& p);
	~SimplePath();
	
	void reset();
	
	void        setNext(SimplePath* next); // next = new SimplePath();
	SimplePath* getNext();
	
	void setPos(const Vec2& p);
	Vec2 getPos() const;
	
private:
	SimplePath* mNext;
	Vec2 mPos;
};
