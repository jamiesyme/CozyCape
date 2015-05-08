#include "healthhud.hpp"
#include "player.hpp"
#include "commongl.hpp"
#include <GL/gl.h>



HealthHud::HeartHud::HeartHud(Vec2 pos)
{
	this->pos            = pos;
	this->mid            = 40.0f;
	this->level          = 0.0f;
	this->lastHealthMove = 0;
}

void HealthHud::HeartHud::onTick(float dt)
{
	animTimer.addTime(dt);
}

void HealthHud::HeartHud::setLevel(float level) 
{
	if (level == this->level)
		return;
	animTimer.setGoal(0.1f);
	animTimer.reset();
	if (level < this->level)
		lastHealthMove = -1;
	else
		lastHealthMove = 1;
	this->level = level;
}

Vec2 HealthHud::HeartHud::getAnimPos() const
{
	Vec2 pos = this->pos;
	if (animTimer.reachedGoal())
		return pos;
	float y = animTimer.timeUntilGoal() / animTimer.getGoal() * 2.0f;
	if (y > 1.0f)
		y = 2.0f - y;
	y *= 30.0f;
	y *= (float)lastHealthMove;
	return Vec2(pos.x, pos.y - y);
}



HealthHud::HealthHud(Player* p)
{
	mPlayer = p;
	mHeart1 = new HeartHud(Vec2( 30.0f, 50.0f));
	mHeart2 = new HeartHud(Vec2( 90.0f, 50.0f));
	mHeart3 = new HeartHud(Vec2(150.0f, 50.0f));
	setType("health hud");
}

HealthHud::~HealthHud()
{
	delete mHeart1;
	delete mHeart2;
	delete mHeart3;
}

void HealthHud::onInit()
{
	getTickGod()->addTo("hud", this);
	getDrawGod()->addTo("hud", this);
}

void HealthHud::onKill()
{
	getTickGod()->removeFrom("hud", this);
	getDrawGod()->removeFrom("hud", this);
}

void HealthHud::onTick(float dt)
{
	mHeart1->onTick(dt);
	mHeart2->onTick(dt);
	mHeart3->onTick(dt);
	
	if (mPlayer == 0)
		return;
	float pHealth = mPlayer->getHealth();
	
	// Update heart 1
	if (pHealth >= 2.0f)
		mHeart1->setLevel(1.0f);
	else
	if (pHealth >= 1.0f)
		mHeart1->setLevel(0.5f);
	else
		mHeart1->setLevel(0.0f);
		
	// Update heart 2
	if (pHealth >= 4.0f)
		mHeart2->setLevel(1.0f);
	else
	if (pHealth >= 3.0f)
		mHeart2->setLevel(0.5f);
	else
		mHeart2->setLevel(0.0f);
		
	// Update heart 3
	if (pHealth >= 6.0f)
		mHeart3->setLevel(1.0f);
	else
	if (pHealth >= 5.0f)
		mHeart3->setLevel(0.5f);
	else
		mHeart3->setLevel(0.0f);
}

void HealthHud::onDraw()
{
	// Get the positions
	Vec2 pos1 = mHeart1->getAnimPos();
	Vec2 pos2 = mHeart2->getAnimPos();
	Vec2 pos3 = mHeart3->getAnimPos();

	// Draw the outlines
	CommonGL::setColor(Color(0.2f, 0.2f, 0.2f, 0.5f));
	_drawHeart(pos1, mHeart1->mid);
	_drawHeart(pos2, mHeart2->mid);
	_drawHeart(pos3, mHeart3->mid);
	
	// Draw the inners
	float offsetMid = 6.0f;
	Vec2  offsetPos = Vec2(0.0f, 4.0f);
	CommonGL::setColor(Color(1.0f, 0.0f, 0.0f, 0.9f));
	
	//   Heart 1
	if (mHeart1->level >= 1.0f)
		_drawHeart(pos1 - offsetPos, mHeart1->mid - offsetMid);
	else
	if (mHeart1->level > 0.0f)
		_drawHalfHeart(pos1 - offsetPos, mHeart1->mid - offsetMid);
		
	//   Heart 2
	if (mHeart2->level >= 1.0f)
		_drawHeart(pos2 - offsetPos, mHeart2->mid - offsetMid);
	else
	if (mHeart2->level > 0.0f)
		_drawHalfHeart(pos2 - offsetPos, mHeart2->mid - offsetMid);
		
	//   Heart 3
	if (mHeart3->level >= 1.0f)
		_drawHeart(pos3 - offsetPos, mHeart3->mid - offsetMid);
	else
	if (mHeart3->level > 0.0f)
		_drawHalfHeart(pos3 - offsetPos, mHeart3->mid - offsetMid);
}

void HealthHud::_drawHeart(Vec2 pos, float midHeight)
{
	CommonGL::push();
	CommonGL::translate(pos);
	float unit = midHeight / 2.0f;
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(-unit, -unit);
		glVertex2f(-unit * 1.25f, -unit * 1.5f);
		glVertex2f(-unit, -unit * 2.0f);
		glVertex2f(-unit * 0.5f, -unit * 2.25f);
		glVertex2f( 0.0f, -unit * 2.0f);
		glVertex2f( unit * 0.5f, -unit * 2.25f);
		glVertex2f( unit, -unit * 2.0f);
		glVertex2f( unit * 1.25f, -unit * 1.5f);
		glVertex2f( unit, -unit);
	glEnd();
	CommonGL::pop();
}

void HealthHud::_drawHalfHeart(Vec2 pos, float midHeight)
{
	CommonGL::push();
	CommonGL::translate(pos);
	float unit = midHeight / 2.0f;
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(-unit, -unit);
		glVertex2f(-unit * 1.25f, -unit * 1.5f);
		glVertex2f(-unit, -unit * 2.0f);
		glVertex2f(-unit * 0.5f, -unit * 2.25f);
		glVertex2f( 0.0f, -unit * 2.0f);
	glEnd();
	CommonGL::pop();
}
