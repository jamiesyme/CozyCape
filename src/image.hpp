#pragma once

#include "color.hpp"
#include <string>

class Image {
public:
	Image();
	Image(int w, int h);
	Image(const Image& i);
	~Image();
	void operator=(const Image& i);
	bool operator==(const Image& i) const;
	bool operator!=(const Image& i) const;
	Color& operator()(int i);
	Color& operator()(int x, int y);
	
	void clear();
	
	void setSize(int w, int h);
	int  getSizeX() const;
	int  getSizeY() const;
	int  getPixelCount() const;
	
	void setPixel(int i, const Color& c);
	void setPixel(int x, int y, const Color& c);
	Color getPixel(int i) const;
	Color getPixel(int x, int y) const;
	Color* getPixels();
	
	// NOTE: Only PNGs supported right now!
	bool load(const std::string& filename);

private:
	Color* mPixels;
	int    mSizeX;
	int    mSizeY;
};
