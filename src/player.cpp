#include "player.hpp"
#include "bow.hpp"
#include "sword.hpp"
#include "keyboard.hpp"
#include "camera.hpp"
#include "commongl.hpp"
#include <cmath>

Player::Player()
{
	mHealth = 6.0f;
	setType("player");
	setBodyCircle(0.4f);
	setBodyGo(this);
}

Player::~Player()
{
}

void Player::onInit()
{
	getTickGod()->addTo("players", this);
	getDrawGod()->addTo("players", this);
	getTriggerGod()->addTo("game", this);
	getPhysicsGod()->add(this);
	
	mBow = new Bow(this);
	getGod()->manage(mBow);
	mSword = new Sword(this);
	getGod()->manage(mSword);
	mWeapon = mBow;
}

void Player::onKill()
{
	getGod()->remove(mBow);
	getTickGod()->removeFrom("players", this);
	getDrawGod()->removeFrom("players", this);
	getTriggerGod()->removeFrom("game", this);
	getPhysicsGod()->remove(this);
}

class TempDeathScreen : public GameObject, public Drawable {
public:
	void onInit() {
		getDrawGod()->addTo("post", this);
	}
	void onKill() {
		getDrawGod()->removeFrom("post", this);
	}
	void onDraw() {
		CommonGL::setColor(Color::Red);
		CommonGL::drawRect(Vec2(-1000.0f, -1000.0f), Vec2(1000.0f, 1000.0f));
	}
};

void Player::onTick(float dt)
{
	// Kill ourself
	static bool madeDeathScreen = false;
	if (mHealth <= 0.0f) {
		if (!madeDeathScreen) {
			getGod()->manage(new TempDeathScreen());
			madeDeathScreen = true;
		}
		return;
	}
	
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
		translate(moveVec * getSpeed() * dt);
	}
}

void Player::onDraw()
{
	CommonGL::setColor(Color::Black);
	CommonGL::drawCircle(getPosition(), getBodyRadius(), 24);
	CommonGL::setColor(Color::White);
	CommonGL::drawCircle(getPosition(), getBodyRadius() * 0.8f, 24);
}

void Player::onMessage(const std::string& msg, GameObject* go)
{
	if (msg == "hit by enemy") 
	{
		damage(1.0f);
	}
}

void Player::onMouseMove(const int x, const int y)
{
	// Get relative coordinates
	Camera* camera = getGod()->getDrawGod()->getCamera("main");
	const Vec2 wCoords = camera->getWorldPos(x, y);
	
	// Aim ourselves
	Vec2 dir = (wCoords - getPosition()).normalized();
	if (dir.length2() == 0.0f) 
		dir.x = 1.0f;
	setRotation(std::atan2(dir.y, dir.x));
}

void Player::onMouseDown(const std::string& button)
{
	if (button == "left")
		if (mWeapon != 0)
			mWeapon->ready();
}

void Player::onMouseUp(const std::string& button)
{
	if (button == "left")
		if (mWeapon != 0)
			mWeapon->attack();
}

float Player::getSpeed() const
{
	return 5.0f;
}

void Player::damage(const float dmg)
{
	mHealth -= dmg;
}

float Player::getHealth() const
{
	return mHealth;
}
