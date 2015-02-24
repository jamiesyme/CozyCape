#pragma once

#include "eventear.hpp"

class Trigger : public EventEar {
public:
	Trigger() {}
	virtual ~Trigger() {}
	
	// All the functions are covered in EventEar.
	// A trigger is essentially just an EventEar.
	// This isn't strict, but it's just handy to keep it like this for now.

private:
	
};
