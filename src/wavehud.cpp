#include "wavehud.hpp"
#include "wavebrain.hpp"
#include "fonthelper.hpp"
#include "window.hpp"
#include <sstream>

WaveHud::WaveHud()
{
}

WaveHud::~WaveHud()
{
}

void WaveHud::onInit()
{
	getDrawGod()->addTo("hud", this);
	WaveBrain* wb = (WaveBrain*)getGod()->findByType("wave brain");
	if (wb != 0) {
		wb->addListener(this);
		onMessage("new wave", wb);
	}
}

void WaveHud::onKill()
{
	getDrawGod()->removeFrom("hud", this);
	WaveBrain* wb = (WaveBrain*)getGod()->findByType("wave brain");
	if (wb != 0) 
		wb->removeListener(this);
}

void WaveHud::onDraw()
{
	const float y = (float)(Window::getHeight() - 1 - mTex.getHeight());
	mTex.onDraw(Vec2(10.0f, y));
}

void WaveHud::onMessage(const std::string& msg, GameObject* go)
{
	if (msg == "new wave") {
		std::stringstream ss;
		ss << "Wave: ";
		ss << ((WaveBrain*)go)->getWave();
		FontHelper::render(ss.str(), 
		                   "Vera", 48, Color::Red, 
		                   mTex);
	}
}
