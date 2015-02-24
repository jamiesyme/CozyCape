#pragma once

#include "gameobject.hpp"
#include "body.hpp"
#include "drawable.hpp"

class Tiles : public GameObject, 
              public Body, 
              public Drawable {
public:
	Tiles();
	Tiles(int sizeX, int sizeY);
	~Tiles();
	
	void onInit();
	void onKill();
	void onDraw();
	
	void load(const std::string& filename);
	
	void reset(int sizeX, int sizeY);	
		
	void setTile(int x, int y, int t);
	int  getTile(int x, int y) const;
	int  getTileAt(const Vec2& p) const;
	
	int getSizeX() const;
	int getSizeY() const;
	
	bool fixBodyCollision(Body* b);
	bool  doRaycast(Raycast& ray);

private:
	int* mTiles;
	int  mSizeX;
	int  mSizeY;
};
