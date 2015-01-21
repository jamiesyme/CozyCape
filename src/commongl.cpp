#include "commongl.hpp"
#include <GL/gl.h>
#include <cmath>

namespace CommonGL {
	void setColor(Color c)
	{
		glColor4ub(c.r, c.g, c.b, c.a);
	}
	
	void setBgColor(Color c)
	{
		glClearColor((float)c.r / 255.0f,
								 (float)c.g / 255.0f,
								 (float)c.b / 255.0f,
								 (float)c.a / 255.0f);
	}
	
	void setOrtho(const Vec2& x, const Vec2& y, const Vec2& z)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(x[0], x[1], y[0], y[1], z[0], z[1]);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	
	void clearColor()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	void clearColorAndDepth()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void push()
	{
		glPushMatrix();
	}
	
	void pop()
	{
		glPopMatrix();
	}
	
	void translate(const Vec2& t)
	{
		glTranslatef(t.x, t.y, 0.0f);
	}
	
	void rotateZ(const float deg)
	{
		glRotatef(deg, 0.0f, 0.0f, 1.0f);
	}
	
	void drawRect(const Vec2& v1, const Vec2& v2)
	{
		glBegin(GL_TRIANGLES);
			glVertex2f(v1.x, v1.y);
			glVertex2f(v2.x, v1.y);
			glVertex2f(v2.x, v2.y);
			glVertex2f(v2.x, v2.y);
			glVertex2f(v1.x, v2.y);
			glVertex2f(v1.x, v1.y);
		glEnd();
	}
	
	void drawCircle(const Vec2& p, const float radius, const int segs)
	{
		const float kFullRad = 3.141592654f * 2.0f;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(p.x, p.y);
		for (int i = 0; i <= segs; i++)
		{
			const float rad = (float)i / (float)segs * kFullRad;
			glVertex2f(p.x + std::cos(rad) * radius,
								 p.y + std::sin(rad) * radius);
		}
		glEnd();
	}
}
