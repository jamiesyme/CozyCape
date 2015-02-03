#include "player.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "camera.hpp"
#include "bullet.hpp"
#include "game.hpp"
#include "map.hpp"
#include "events.hpp"
#include <cmath>

Player::Player()
{
	mTickTimer.set(0.0);
	mShotTimer.set(0.0);
	mIsShotHeld = false;
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
}

void Player::onMouseMove(const int x, const int y)
{
	// Get relative coordinates
	const Vec2 wCoords = Game::getCamera()->getWorldPos(x, y);
	
	// Aim ourselves
	Vec2 dir = (wCoords - getPos()).normalized();
	if (dir.length2() == 0.0f) dir.x = 1.0f;
	setRot(std::atan2(dir.y, dir.x));
}

void Player::onMouseDown(const std::string& button)
{
	if (button == "left")
		pullShot();
}

void Player::onMouseUp(const std::string& button)
{
	if (button == "left")
		releaseShot();
}

void Player::pullShot()
{
	if (mIsShotHeld)
		return;
	mIsShotHeld = true;
	mShotTimer.set(0.0);
}

void Player::releaseShot()
{
	if (!mIsShotHeld)
		return;
		
	// Get the power
	const float shotPower = getShotPower();
	mIsShotHeld = false;
	
	// Calculate the direction
	Vec2 dir = Vec2(std::cos(getRot()), std::sin(getRot()));
	
	// Shoot
	Bullet* bullet = new Bullet();
	bullet->setBounceMax(0);
	bullet->setDirection(dir);
	bullet->setPos(getPos() + dir * getRadius());
	bullet->setDuration(shotPower);
	bullet->fire();
	Map::manageEntity(bullet);
}

bool Player::isShotHeld() const
{
	return mIsShotHeld;
}

float Player::getShotPower() const
{
	if (!mIsShotHeld)
		return 0.0f;
	if (mShotTimer.get() <= 0.2)
		return 0.2f;
	if (mShotTimer.get() >= 1.0)
		return 1.0f;
	return mShotTimer.get();
}
