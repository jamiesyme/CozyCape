#pragma once

#include <vector>
#include <string>
class Entity;
class Ray;
class RaycastInfo;

class EntityManager {
public:
	EntityManager();
	~EntityManager();
	
	void manage(Entity* e);
	void remove(Entity* e);
	
	Entity* findByType(const std::string& type);
	
	void onTick();
	void onDraw();
	void onPhysics();
	
	bool doRaycast(const Ray& ray, RaycastInfo* info);
	
private:
	std::vector<Entity*> mEnts;
	std::vector<int>     mEntStates;
	
	void _onStateTick();
	void _solveComplex(Entity* c,  Entity* d);
	void _solveDynamic(Entity* d1, Entity* d2);
	bool _raycastCircle(Entity* e, const Ray& r, RaycastInfo* i);
};
