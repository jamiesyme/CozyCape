#pragma once

class Counter {
public:
	Counter();
	Counter(const float goal);
	~Counter();
	
	void  reset();
	void  addTime(const float dt);
	float getTime() const;
	
	void  setGoal(const float goal);
	float getGoal()       const;
	bool  reachedGoal()   const;
	float timeUntilGoal() const;
	void  forceToGoal();

private:
	float mTime;
	float mGoal;
};
