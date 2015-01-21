#include "clock.hpp"
#include <SDL2/SDL.h>
#include <thread>
#include <chrono>

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
	
	void sleepFor(double time) {
		if (time <= 0.0)
			return;
		const int milli = (int)(time * 1000.0);
		std::this_thread::sleep_for(std::chrono::milliseconds(milli));
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
