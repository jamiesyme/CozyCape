#pragma once

class Tickable {
public:
	Tickable() {}
	virtual ~Tickable() {}

	virtual void onTick(float dt) = 0;

private:
	
};
