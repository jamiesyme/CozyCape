#include "counter.hpp"

Counter::Counter()
{
	mTime = 0.0f;
	mGoal = 0.0f;
}

Counter::Counter(const float goal)
{
	mTime = 0.0f;
	mGoal = goal;
}

Counter::~Counter()
{
}

void Counter::reset()
{
	mTime = 0.0f;
}

void Counter::addTime(const float dt)
{
	mTime += dt;
}

float Counter::getTime() const
{
	return mTime;
}

void Counter::setGoal(const float goal)
{
	mGoal = goal;
}

float Counter::getGoal() const
{
	return mGoal;
}

bool Counter::reachedGoal() const
{
	return (mTime >= mGoal);
}

float Counter::timeUntilGoal() const
{
	if (reachedGoal())
		return 0.0f;
	return mGoal - mTime;
}

void Counter::forceToGoal()
{
	if (reachedGoal())
		return;
	mTime = mGoal;
}
