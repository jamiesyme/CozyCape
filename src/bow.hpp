#pragma once

#include "weapon.hpp"
#include "gameobject.hpp"
class Body;

class Bow : public Weapon,
            public GameObject {
public:
	Bow(Body* user);
	~Bow();
	
	void ready();
	void attack();

private:
	Body* mUser;
};
