#pragma once

#include "eventear.hpp"

namespace Events {
	void poll();
	
	void    addEar(EventEar* e);
	void removeEar(EventEar* e);
}
