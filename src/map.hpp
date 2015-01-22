#pragma once

class Entity;

namespace Map {
	void init(int w, int h);
	void kill();
	
	void onTick();
	void onDraw();
	void onPhysics();
	
	void  manageEntity(Entity* e);
	void destroyEntity(Entity* e);
	
	void setTile(int x, int y, int t);
	int  getTile(int x, int y);
	
	void setSize(int w, int h);
	int getWidth();
	int getHeight();
}
