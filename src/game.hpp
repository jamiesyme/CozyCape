#pragma once

class Entity;

namespace Game {
	void run();
	void stop();
	
	void  manageEntity(Entity* e);
	void destroyEntity(Entity* e);
}
