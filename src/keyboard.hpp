#pragma once

#include <string>

namespace Keyboard {
	void init();
	void kill();
	
	bool isKeyDown(const std::string& key);
};
