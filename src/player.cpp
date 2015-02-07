#include "player.hpp"
#include "keyboard.hpp"
#include "camera.hpp"
#include "arrow.hpp"
#include "game.hpp"
#include "events.hpp"
#include "commongl.hpp"
#include <cmath>

Player::Player()
{
	mTickTimer.set(0.0);
	mShotTimer.set(0.0);
	mIsArrowHeld = false;
	setBodyCircle(0.4f);
	setType("player");
	Events::addEar(this);
}

Player::~Player()
{
	Events::removeEar(this);
}

void Player::onTick()
{
	const float speed = 6.0f;
	const float dt = (float)mTickTimer.get();
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
		translate(moveVec * speed * dt);
	}
}

void Player::onDraw()
{
	CommonGL::setColor(Color::Black);
	CommonGL::drawCircle(getPosition(), getBodyRadius(), 24);
	CommonGL::setColor(Color::White);
	CommonGL::drawCircle(getPosition(), getBodyRadius() * 0.8f, 24);
}

void Player::onMouseMove(const int x, const int y)
{
	// Get relative coordinates
	const Vec2 wCoords = Game::getCamera()->getWorldPos(x, y);
	
	// Aim ourselves
	Vec2 dir = (wCoords - getPosition()).normalized();
	if (dir.length2() == 0.0f) dir.x = 1.0f;
	setRotation(std::atan2(dir.y, dir.x));
}

void Player::onMouseDown(const std::string& button)
{
	if (button == "left")
		pullArrow();
}

void Player::onMouseUp(const std::string& button)
{
	if (button == "left")
		releaseArrow();
}

void Player::pullArrow()
{
	if (mIsArrowHeld)
		return;
	mIsArrowHeld = true;
	mShotTimer.set(0.0);
}

void Player::releaseArrow()
{
	if (!mIsArrowHeld)
		return;
		
	// Get the power
	const float arrowPower = getArrowPower();
	mIsArrowHeld = false;
	
	// Calculate the direction
	Vec2 dir = Vec2(std::cos(getRotation()), std::sin(getRotation()));
	
	// Shoot
	Arrow* arrow = new Arrow();
	arrow->setDirection(dir);
	arrow->setPosition(getPosition());
	arrow->setDuration(arrowPower);
	arrow->shoot();
	Game::manageEntity(arrow);
}

bool Player::isArrowHeld() const
{
	return mIsArrowHeld;
}

float Player::getArrowPower() const
{
	if (!mIsArrowHeld)
		return 0.0f;
	if (mShotTimer.get() <= 0.2)
		return 0.2f;
	if (mShotTimer.get() >= 1.0)
		return 1.0f;
	return mShotTimer.get();
}
