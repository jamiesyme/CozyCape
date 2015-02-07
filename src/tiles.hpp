#pragma once

#include "entity.hpp"
#include <string>
class PathInfo;

class Tiles : public Entity {
public:
	Tiles();
	Tiles(int sizeX, int sizeY);
	~Tiles();
	
	void onDraw();
	
	void load(const std::string& filename);
	
	void reset(int sizeX, int sizeY);	
		
	void setTile(int x, int y, int t);
	int  getTile(int x, int y) const;
	
	int getSizeX() const;
	int getSizeY() const;
	
	bool fixComplexBodyCollision(Entity* e);
	bool  doComplexBodyRaycast(const Ray& ray, RaycastInfo* info);

private:
	int* mTiles;
	int  mSizeX;
	int  mSizeY;
};
