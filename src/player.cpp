#include "player.hpp"
#include "keyboard.hpp"
#include "camera.hpp"
#include "arrow.hpp"
#include "game.hpp"
#include "entitymanager.hpp"
#include "events.hpp"
#include "commongl.hpp"
#include <cmath>

Player::Player()
{
	mTickTimer.set(0.0);
	mShotTimer.set(0.0);
	mIsArrowHeld = false;
	mHealth = 100.0f;
	setType("player");
	setBodyCircle(0.4f);
	setDepth(-0.5f);
	Events::addEar(this);
}

Player::~Player()
{
	Events::removeEar(this);
}

class TempDeathScreen : public Entity {
public:
	void onDraw() {
		CommonGL::translateZ(-0.1f);
		CommonGL::setColor(Color::Red);
		CommonGL::drawRect(Vec2(-1000.0f, -1000.0f), Vec2(1000.0f, 1000.0f));
		CommonGL::translateZ(0.1f);
	}
};

void Player::onTick()
{
	// Kill ourself
	static bool madeDeathScreen = false;
	if (mHealth < 0.0f) {
		if (!madeDeathScreen)
			getManager()->manage(new TempDeathScreen());
		return;
	}

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
	CommonGL::push();
	CommonGL::translateZ(getDepth());
	CommonGL::setColor(Color::Black);
	CommonGL::drawCircle(getPosition(), getBodyRadius(), 24);
	CommonGL::translateZ(0.05f);
	CommonGL::setColor(Color::White);
	CommonGL::drawCircle(getPosition(), getBodyRadius() * 0.8f, 24);
	CommonGL::pop();
}

void Player::onMessage(const std::string& s, void* d)
{
	if (s == "hit by enemy") 
	{
		damage(10.0f);
	}
}

void Player::onMouseMove(const int x, const int y)
{
	// Get relative coordinates
	const Vec2 wCoords = Game::getCamera()->getWorldPos(x, y);
	
	// Aim ourselves
	Vec2 dir = (wCoords - getPosition()).normalized();
	if (dir.length2() == 0.0f) 
		dir.x = 1.0f;
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
	//const float arrowPower = getArrowPower();
	mIsArrowHeld = false;
	
	// Shoot
	Arrow* arrow = new Arrow();
	arrow->setRotation(getRotation());
	arrow->setPosition(getPosition());
	getManager()->manage(arrow);
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

void Player::damage(const float dmg)
{
	mHealth -= dmg;
}
