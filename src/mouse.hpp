#pragma once

#include <string>

namespace Mouse {
	void init();
	void kill();
	
	int  getX();
	int  getY();
	bool isButtonDown(const std::string& button);
};
