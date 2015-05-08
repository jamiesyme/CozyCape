#pragma once

class Weapon {
public:
	Weapon() {}
	~Weapon() {}
	
	virtual void ready() = 0;  // Ex. Pull back bow string
	virtual void attack() = 0; // Ex. Release bow string

private:
	
};
