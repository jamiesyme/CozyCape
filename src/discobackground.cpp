#include "discobackground.hpp"
#include "commongl.hpp"
#include <cstdlib>

DiscoBackground::DiscoBackground()
{
	mOldColor = Color::Black;
	mNewColor = Color::Black;
	mBlendTimer.setGoal(3.0f);
	mChangeTimer.setGoal(6.0f);
	mChangeTimer.forceToGoal();
}

DiscoBackground::~DiscoBackground()
{
}

void DiscoBackground::onInit()
{
	getTickGod()->addTo("game", this);
}

void DiscoBackground::onKill()
{
	getTickGod()->removeFrom("game", this);
}

void DiscoBackground::onTick(float dt)
{
	mBlendTimer.addTime(dt);
	mChangeTimer.addTime(dt);
	
	if (mChangeTimer.reachedGoal()) {
		mBlendTimer.reset();
		mChangeTimer.reset();
		
		mOldColor = mNewColor;
		mNewColor.r = (float)rand() / (float)RAND_MAX * 255.0f / 2.0f;
		mNewColor.g = (float)rand() / (float)RAND_MAX * 255.0f / 2.0f;
		mNewColor.b = (float)rand() / (float)RAND_MAX * 255.0f / 2.0f;
	}
	
	float ratio = 1.0f;
	if (!mBlendTimer.reachedGoal())
		ratio = mBlendTimer.getTime() / mBlendTimer.getGoal();
		
	Color mixed = mOldColor;
	mixed.r += (float)(mNewColor.r - mOldColor.r) * ratio;
	mixed.g += (float)(mNewColor.g - mOldColor.g) * ratio;
	mixed.b += (float)(mNewColor.b - mOldColor.b) * ratio;
	CommonGL::setBgColor(mixed);
}
