#pragma once

#include "gameobject.hpp"
#include "drawable.hpp"
class Player;
class Vec2;

class HealthHud : public GameObject,
                  public Drawable {
public:
	HealthHud(Player* p);
	~HealthHud();
	
	void onInit();
	void onKill();
	void onDraw();

private:
	Player* mPlayer;
	
	void _drawHeart(Vec2 pos, float midHeight);
	void _drawHalfHeart(Vec2 pos, float midHeight);
};
