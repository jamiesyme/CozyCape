#include "player.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "camera.hpp"
#include "bullet.hpp"
#include "game.hpp"
#include "map.hpp"
#include "events.hpp"

Player::Player()
{
	mTickTimer.set(0.0);
	mShotTimer.set(0.0);
	setRadius(0.4f);
	Events::addEar(this);
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
	if (Mouse::isButtonDown("left"))
		shoot();
}

void Player::onMouseDown(const std::string& button)
{
	if (button == "left")
		shoot();
}

void Player::shoot()
{
	if (mShotTimer.get() > 0.1) 
	{
		mShotTimer.set(0.0);
		
		Camera* cam = Game::getCamera();
		Vec2 mousePos  = cam->getWorldPos(Mouse::getX(), Mouse::getY());
		Vec2 direction = (mousePos - getPos()).normalized();
		
		Bullet* bullet = new Bullet();
		bullet->setBounceMax(0);
		bullet->setDirection(direction);
		bullet->setPos(getPos() + direction * getRadius());
		bullet->fire();
		Map::manageEntity(bullet);
	}
}
