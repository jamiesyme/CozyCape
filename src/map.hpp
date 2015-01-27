#pragma once

#include "vec2.hpp"
class Entity;

namespace Map {
	struct Raycast {
		const float kMaxDist = -1.0f;
		Vec2  pos;
		Vec2  dir;
		float dist = kMaxDist;
	};
	struct RaycastHitTile {
		int   tx;
		int   ty;
		int   tile;
		Vec2  normal;
		Vec2  point;
		float dist;
	};
	struct RaycastHitEntity {
		Entity* ent;
		Vec2    normal;
		Vec2    point;
		float   dist;
	};

	void init(int w, int h);
	void kill();
	
	void onTick();
	void onDraw();
	
	void onPhysics();
	bool   tileRaycast(const Raycast& ray, RaycastHitTile* hitInfo);
	bool entityRaycast(const Raycast& ray, RaycastHitEntity* hitInfo);
	
	void  manageEntity(Entity* e);
	void destroyEntity(Entity* e);
	
	void setTile(int x, int y, int t);
	int  getTile(int x, int y);
	
	void setSize(int w, int h);
	int getWidth();
	int getHeight();
}
