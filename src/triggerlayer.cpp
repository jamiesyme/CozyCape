#include "triggerlayer.hpp"

TriggerLayer::TriggerLayer(const std::string& name)
{
	mName = name;
}

TriggerLayer::~TriggerLayer()
{
}

std::string TriggerLayer::getName() const
{
	return mName;
}

void TriggerLayer::onKeyDown(const std::string& s)
{
	for (unsigned int i = 0; i < mTriggers.size(); i++)
		mTriggers[i]->onKeyDown(s);
}

void TriggerLayer::onKeyUp(const std::string& s)
{
	for (unsigned int i = 0; i < mTriggers.size(); i++)
		mTriggers[i]->onKeyUp(s);
}

void TriggerLayer::onMouseDown(const std::string& s)
{
	for (unsigned int i = 0; i < mTriggers.size(); i++)
		mTriggers[i]->onMouseDown(s);
}

void TriggerLayer::onMouseUp(const std::string& s)
{
	for (unsigned int i = 0; i < mTriggers.size(); i++)
		mTriggers[i]->onMouseUp(s);
}

void TriggerLayer::onMouseMove(const int x, const int y)
{
	for (unsigned int i = 0; i < mTriggers.size(); i++)
		mTriggers[i]->onMouseMove(x, y);
}

void TriggerLayer::onMouseScroll(const int i)
{
	for (unsigned int i = 0; i < mTriggers.size(); i++)
		mTriggers[i]->onMouseScroll(i);
}

void TriggerLayer::onWindowResize(const int w, const int h)
{
	for (unsigned int i = 0; i < mTriggers.size(); i++)
		mTriggers[i]->onWindowResize(w, h);
}

void TriggerLayer::onWindowClose()
{
	for (unsigned int i = 0; i < mTriggers.size(); i++)
		mTriggers[i]->onWindowClose();
}

void TriggerLayer::add(Trigger* t)
{
	if (t != 0)
		mTriggers.push_back(t);
}

void TriggerLayer::remove(Trigger* t)
{
	for (unsigned int i = 0; i < mTriggers.size(); i++) {
		if (t == mTriggers[i]) {
			mTriggers.erase(mTriggers.begin() + i);
			return;
		}
	}
}

void TriggerLayer::removeAll()
{
	mTriggers.clear();
}
