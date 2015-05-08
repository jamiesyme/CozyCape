#include "texture.hpp"
#include "image.hpp"
#include <GL/gl.h>

Texture::Texture()
{
	mId = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &mId);
}

void Texture::load(Image& img)
{
	// Generate the texture id
	if (mId == 0)
		glGenTextures(1, &mId);
		
	// Load it
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 
	             img.getSizeX(), img.getSizeY(), 0, 
	             GL_RGBA, GL_UNSIGNED_BYTE, img.getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unbind();
	
	// Save the size
	mSize = Vec2((float)img.getSizeX(), (float)img.getSizeY());
}

void Texture::onDraw(const Vec2& pos) const
{
	onDraw(pos, getSize());
}

void Texture::onDraw(const Vec2& pos, const Vec2& size) const
{
	if (mId == 0)
		return;
	glColor3f(1.0f, 1.0f, 1.0f);
	bind();
	glBegin(GL_TRIANGLES);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(pos.x,          pos.y);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(pos.x + size.x, pos.y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(pos.x + size.x, pos.y + size.y);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(pos.x + size.x, pos.y + size.y);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(pos.x,          pos.y + size.y);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(pos.x,          pos.y);
	glEnd();
	unbind();
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, mId);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Vec2 Texture::getSize() const
{
	return mSize;
}

int Texture::getWidth() const
{
	return (int)mSize.x;
}

int Texture::getHeight() const
{
	return (int)mSize.y;
}
