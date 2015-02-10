#pragma once

class EntityManager;
class Camera;

namespace Game {
	void run();
	void stop();
	
	void    setCamera(Camera* c);
	Camera* getCamera();
	
	EntityManager* getEntityManager();
}
