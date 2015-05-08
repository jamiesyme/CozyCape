#pragma once

#include "gameobject.hpp"
#include "tickable.hpp"
#include "counter.hpp"
#include "color.hpp"

class DiscoBackground : public GameObject,
                        public Tickable {
public:
	DiscoBackground();
	~DiscoBackground();
	
	void onInit();
	void onKill();
	void onTick(float dt);

private:
	Color   mOldColor;
	Color   mNewColor;
	Counter mBlendTimer;
	Counter mChangeTimer;
};
