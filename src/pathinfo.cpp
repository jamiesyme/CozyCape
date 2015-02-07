#include "pathinfo.hpp"

PathInfo::PathInfo()
{
}

PathInfo::~PathInfo()
{
}

void PathInfo::append(const Vec2& n)
{
	mNodes.push_back(n);
}

void PathInfo::push(const Vec2& n)
{
	mNodes.insert(mNodes.begin(), n);
}

void PathInfo::clear()
{
	mNodes.clear();
}

Vec2 PathInfo::get(int i) const
{
	if (i < 0 || i >= (int)mNodes.size())
		return Vec2();
	return mNodes[i];
}

int PathInfo::getCount() const
{
	return (int)mNodes.size();
}
