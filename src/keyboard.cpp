#include "keyboard.hpp"
#include "eventear.hpp"
#include "events.hpp"
#include <map>

namespace {
	std::map<std::string, bool> gKeys;

	class KeyListener : public EventEar {
	public:
		void onKeyDown(const std::string& s) {
			gKeys[s] = true;
		}
		void onKeyUp(const std::string& s) {
			gKeys[s] = false;
		}
	};
	KeyListener gEar;
}

void Keyboard::init()
{
	gKeys.clear();
	Events::addEar(&gEar);
}

void Keyboard::kill()
{
	Events::removeEar(&gEar);
	gKeys.clear();
}

bool Keyboard::isKeyDown(const std::string& key)
{
	if (gKeys.count(key) > 0)
		return gKeys[key];
	return false;
}
