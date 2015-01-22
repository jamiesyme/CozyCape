#include "player.hpp"
#include "keyboard.hpp"

Player::Player()
{
	mTickTimer.set(0.0);
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
		
	if (moveVec.getSqrMag() != 0.0f) {
		moveVec.normalize();
		move(moveVec * 6.0f * dt);
	}
}
