#include "mouse.hpp"
#include "eventear.hpp"
#include "events.hpp"
#include <map>

namespace {
	int gX;
	int gY;
	std::map<std::string, bool> gButtons;

	class MouseListener : public EventEar {
	public:
		void onMouseMove(const int x, const int y) {
			gX = x;
			gY = y;
		}
		void onMouseDown(const std::string& s) {
			gButtons[s] = true;
		}
		void onMouseUp(const std::string& s) {
			gButtons[s] = false;
		}
	};
	MouseListener gEar;
}

void Mouse::init()
{
	gButtons.clear();
	Events::addEar(&gEar);
}

void Mouse::kill()
{
	Events::removeEar(&gEar);
	gButtons.clear();
}

int Mouse::getX()
{
	return gX;
}

int Mouse::getY()
{
	return gY;
}

bool Mouse::isButtonDown(const std::string& button)
{
	if (gButtons.count(button) > 0)
		return gButtons[button];
	return false;
}
