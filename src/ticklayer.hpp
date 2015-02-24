#pragma once

#include <vector>
#include <string>
#include "tickable.hpp"

class TickLayer : public Tickable {
public:
	TickLayer(const std::string& name);
	~TickLayer();
	
	std::string getName() const;
	
	void onTick(float dt);
	
	void    add(Tickable* t);
	void remove(Tickable* t);
	void removeAll();

private:
	std::string mName;
	std::vector<Tickable*> mTickables;
};
