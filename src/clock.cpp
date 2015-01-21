#include "clock.hpp"
#include <SDL2/SDL.h>

static Clock::Timer gTimer;

namespace Clock {
	double getRawTime() {
		return (double)SDL_GetTicks() / 1000.0;
	}

	double getTime() { 
		return gTimer.get(); 
	}

	void setTime(double time) { 
		gTimer.set(time); 
	}
	
	
	Timer::Timer()
	{
		set(0.0);
	}

	Timer::~Timer()
	{
	}

	double Timer::get() const
	{
		return getRawTime() + mOffset;
	}

	void Timer::set(double t)
	{
		mOffset = t - getRawTime();
	}
}
