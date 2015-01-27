#include "player.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "camera.hpp"
#include "bullet.hpp"
#include "game.hpp"
#include "map.hpp"

Player::Player()
{
	mTickTimer.set(0.0);
	mShotTimer.set(0.0);
	setRadius(0.4f);
}

Player::~Player()
{
}

void Player::onTick()
{
	float dt = (float)mTickTimer.get();
	mTickTimer.set(0.0);
	
	Vec2 moveVec;
	if (Keyboard::isKeyDown("w"))
		moveVec.y -= 1;
	if (Keyboard::isKeyDown("s"))
		moveVec.y += 1;
	if (Keyboard::isKeyDown("d"))
		moveVec.x += 1;
	if (Keyboard::isKeyDown("a"))
		moveVec.x -= 1;
		
	if (moveVec.length2() != 0.0f) {
		moveVec.normalize();
		move(moveVec * 6.0f * dt);
	}
	
	// Shoot bullet
	if (Mouse::isButtonDown("left") && mShotTimer.get() > 0.1f) 
	{
		mShotTimer.set(0.0);
	
		Camera* cam = Game::getCamera();
		Vec2 mousePos  = cam->getWorldPos(Mouse::getX(), Mouse::getY());
		Vec2 direction = (mousePos - getPos()).normalized();
		
		Bullet* bullet = new Bullet();
		bullet->setDirection(direction);
		bullet->setPos(getPos() + direction * getRadius());
		bullet->fire();
		Map::manageEntity(bullet);
	}
}
