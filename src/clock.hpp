#pragma once

namespace Clock {
	double getRawTime();
	double getTime();
	void   setTime(double time);
	void   sleepFor(double time);
	
	
	class Timer {
	public:
		Timer();
		~Timer();
		
		double get() const;
		void   set(double t);
		
	private:
		double mOffset;
	};
}
