#pragma once

class Camera;
class Player;

namespace Game {
	void run();
	void stop();
	
	void    setCamera(Camera* c);
	Camera* getCamera();
	
	void    setPlayer(Player* p);
	Player* getPlayer();
}
