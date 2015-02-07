#pragma once

#include <string>
class Entity;
class Camera;
class Ray;
class RaycastInfo;

namespace Game {
	void run();
	void stop();
	
	void manageEntity(Entity* e);
	void removeEntity(Entity* e);
	
	Entity* findEntityByType(const std::string& type);
	
	bool doRaycast(const Ray& ray, RaycastInfo* info);
	
	void    setCamera(Camera* c);
	Camera* getCamera();
}
