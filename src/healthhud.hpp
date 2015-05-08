#pragma once

#include "gameobject.hpp"
#include "tickable.hpp"
#include "drawable.hpp"
#include "counter.hpp"
class Player;
class Vec2;

class HealthHud : public GameObject,
                  public Tickable,
                  public Drawable {
public:
	HealthHud(Player* p);
	~HealthHud();
	
	void onInit();
	void onKill();
	void onTick(float dt);
	void onDraw();

private:
	struct HeartHud;
	Player* mPlayer;
	HeartHud* mHeart1;
	HeartHud* mHeart2;
	HeartHud* mHeart3;
	
	void _drawHeart(Vec2 pos, float midHeight);
	void _drawHalfHeart(Vec2 pos, float midHeight);
	
	struct HeartHud {
		Vec2    pos;
		float   mid;
		float   level;
		Counter animTimer;
		int     lastHealthMove;
		
		HeartHud(Vec2 pos);
		void onTick(float dt);
		void setLevel(float level);
		Vec2 getAnimPos() const;
	};
};
