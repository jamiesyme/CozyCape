#pragma once

#include "gameobject.hpp"
#include "drawable.hpp"
#include "texture.hpp"

class WaveHud : public GameObject, 
                public Drawable {
public:
	WaveHud();
	~WaveHud();
	
	void onInit();
	void onKill();
	void onDraw();
	void onMessage(const std::string& msg, GameObject* go);

private:
	Texture mTex;
};
