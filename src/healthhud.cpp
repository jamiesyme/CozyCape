#include "healthhud.hpp"
#include "player.hpp"
#include "commongl.hpp"
#include "drawgod.hpp"
#include <GL/gl.h>

HealthHud::HealthHud(Player* p)
{
	mPlayer = p;
	setType("health hud");
}

HealthHud::~HealthHud()
{
}

void HealthHud::onInit()
{
	getDrawGod()->addTo("hud", this);
}

void HealthHud::onKill()
{
	getDrawGod()->removeFrom("hud", this);
}

void HealthHud::onDraw()
{
	CommonGL::setColor(Color(0.2f, 0.2f, 0.2f, 0.5f));
	_drawHeart(Vec2( 30.0f, 50.0f), 40.0f);
	_drawHeart(Vec2( 90.0f, 50.0f), 40.0f);
	_drawHeart(Vec2(150.0f, 50.0f), 40.0f);
	if (mPlayer == 0)
		return;
	CommonGL::setColor(Color(1.0f, 0.0f, 0.0f, 0.9f));
	float health = mPlayer->getHealth();
	if (health >= 2.0f)
		_drawHeart(Vec2( 30.0f, 46.0f), 34.0f);
	else 
	if (health >= 1.0f)
		_drawHalfHeart(Vec2( 30.0f, 46.0f), 34.0f);
	if (health >= 4.0f)
		_drawHeart(Vec2(90.0f, 46.0f), 34.0f);
	else 
	if (health >= 3.0f)
		_drawHalfHeart(Vec2(90.0f, 46.0f), 34.0f);
	if (health >= 6.0f)
		_drawHeart(Vec2(150.0f, 46.0f), 34.0f);
	else 
	if (health >= 5.0f)
		_drawHalfHeart(Vec2(150.0f, 46.0f), 34.0f);
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
