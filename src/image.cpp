#include "image.hpp"
#include "lodepng.h"

Image::Image()
{
	mPixels = 0;
	mSizeX = 0;
	mSizeY = 0;
}

Image::Image(int w, int h)
{
	mPixels = 0;
	mSizeX = 0;
	mSizeY = 0;
	setSize(w, h);
}

Image::Image(const Image& i)
{
	mPixels = 0;
	mSizeX = 0;
	mSizeY = 0;
	*this = i;
}

Image::~Image()
{
	delete[] mPixels;
}

void Image::operator=(const Image& img)
{
	clear();	
	setSize(img.getSizeX(), img.getSizeY());
	
	const int pixelCount = getPixelCount();
	for (int i = 0; i < pixelCount; i++)
		mPixels[i] = img.mPixels[i];
}

bool Image::operator==(const Image& img) const
{
	if (getSizeX() != img.getSizeX()) return false;
	if (getSizeY() != img.getSizeY()) return false;
	
	const int pixelCount = getPixelCount();
	for (int i = 0; i < pixelCount; i++)
		if (mPixels[i] != img.mPixels[i])	
			return false;
	return true;
}

bool Image::operator!=(const Image& img) const
{
	return !(*this == img);
}

Color& Image::operator()(int i)
{
	return mPixels[i];
}

Color& Image::operator()(int x, int y)
{
	return mPixels[y * mSizeX + x];
}

void Image::clear()
{
	delete[] mPixels;
	mPixels = 0;
	mSizeX = 0;
	mSizeY = 0;
}

void Image::setSize(int w, int h)
{
	clear();
	if (w <= 0 || h <= 0)
		return;
		
	mSizeX = w;
	mSizeY = h;
	mPixels = new Color[w * h];
}

int Image::getSizeX() const
{
	return mSizeX;
}

int Image::getSizeY() const
{
	return mSizeY;
}

int Image::getPixelCount() const
{
	return mSizeX * mSizeY;
}

void Image::setPixel(int i, const Color& c)
{
	if (i < 0 || i >= getPixelCount())
		return;
	(*this)(i) = c;
}

void Image::setPixel(int x, int y, const Color& c)
{
	if (x < 0 || x >= mSizeX ||
			y < 0 || y >= mSizeY)
		return;
	(*this)(x, y) = c;
}

Color Image::getPixel(int i) const
{
	if (i < 0 || i >= getPixelCount())
		return Color();
	return this->mPixels[i];
}

Color Image::getPixel(int x, int y) const
{
	if (x < 0 || x >= mSizeX ||
			y < 0 || y >= mSizeY)
		return Color();
	return this->mPixels[y * mSizeX + x];
}

Color* Image::getPixels()
{
	return mPixels;
}

bool Image::load(const std::string& filename)
{
	// Clear the old data
	clear();
	
	// Load the image
	unsigned int width = 0;
	unsigned int height = 0;
	std::vector<unsigned char> pixels;
	if (lodepng::decode(pixels, width, height, filename.c_str()) != 0)
		return false; // We failed
		
	// Save the loaded image data
	setSize(width, height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			const int index = y * width + x;
			mPixels[index].r = pixels[index * 4 + 0];
			mPixels[index].g = pixels[index * 4 + 1];
			mPixels[index].b = pixels[index * 4 + 2];
			mPixels[index].a = pixels[index * 4 + 3];
		}
	}
	return true;
}
