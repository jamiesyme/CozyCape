#include "simplepath.hpp"

SimplePath::SimplePath()
{
	mNext = 0;
}

SimplePath::SimplePath(const Vec2& p)
{
	mNext = 0;
	mPos  = p;
}

SimplePath::~SimplePath()
{
	reset();
}

void SimplePath::reset()
{
	if (mNext != 0) {
		delete mNext;
		mNext = 0;
	}
}

void SimplePath::setNext(SimplePath* next)
{
	// Doesn't delete old next -- this is intended!
	mNext = next;
}

SimplePath* SimplePath::getNext()
{
	return mNext;
}

void SimplePath::setPos(const Vec2& p)
{
	mPos = p;
}

Vec2 SimplePath::getPos() const
{
	return mPos;
}
