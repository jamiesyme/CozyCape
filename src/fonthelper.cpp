#include "fonthelper.hpp"
#include <SDL2/SDL_ttf.h>
#include <vector>

namespace {
	struct Alias {
		std::string original;
		std::string alias;
	};
	std::vector<Alias> mAliases;
	
	struct Font {
		std::string filename;
		TTF_Font*   sdlFont;
		int         size;
	};
	std::vector<Font> mFonts;
}

void FontHelper::init()
{
	TTF_Init();
}

void FontHelper::kill()
{
	mAliases.clear();
	for (unsigned int i = 0; i < mFonts.size(); i++)
		TTF_CloseFont(mFonts[i].sdlFont);
	mFonts.clear();
	TTF_Quit();
}

bool FontHelper::setAlias(const std::string& filename, 
                          const std::string& alias)
{
	// Check for existing alias
	for (unsigned int i = 0; i < mAliases.size(); i++)
		if (mAliases[i].alias == alias)
			return false;
			
	Alias a;
	a.original = filename;
	a.alias    = alias;
	mAliases.push_back(a);
	return true;
}

bool FontHelper::render(const std::string& text,
                        const std::string& fontName,
                        const int size,
                        const Color& color,
                        Image& output)
{
	// Get the font filename (check aliases)
	std::string filename = fontName;
	for (unsigned int i = 0; i < mAliases.size(); i++) {
		if (filename == mAliases[i].alias) {
			filename = mAliases[i].original;
			break;
		}
	}

	// Check if the font is already loaded
	Font* font = 0;
	for (unsigned int i = 0; i < mFonts.size(); i++) {
		if (filename == mFonts[i].filename &&
		    size     == mFonts[i].size) {
			font = &(mFonts[i]);
			break;
		}
	}
	
	// If we didn't find the font, load it
	if (font == 0) {
		Font newFont;
		newFont.filename = filename;
		newFont.size     = size;
		newFont.sdlFont  = TTF_OpenFont(filename.c_str(), size);
		if (newFont.sdlFont == 0)
			return false;              // Failed to load
		mFonts.push_back(newFont);
		font = &(mFonts[mFonts.size() - 1]);
	}
	
	// Render the text
	SDL_Color sdlColor = { 255, 255, 255, 255 };
	SDL_Surface* sdlSurface = TTF_RenderText_Solid(font->sdlFont, 
	                                               text.c_str(), 
	                                               sdlColor);
	
	// Copy the surface to our output image
	Uint8* pixels = (Uint8*)sdlSurface->pixels;
	output.setSize(sdlSurface->w, sdlSurface->h);
	for (int y = 0; y < sdlSurface->h; y++) {
		for (int x = 0; x < sdlSurface->w; x++) {
			Uint8 index = pixels[y * (sdlSurface->pitch / sizeof(Uint8)) + x];
			Color c;
			if (index == 0) c = Color::Transparent;
			else            c = color;
			output.setPixel(x, y, c);
		}
	}
	
	// Free the surface
	SDL_FreeSurface(sdlSurface);
	return true;
}

bool FontHelper::render(const std::string& text, 
	                      const std::string& fontName, 
	                      const int size,
	                      const Color& color,
	                      Texture& output)
{
	Image img;
	if (!render(text, fontName, size, color, img))
		return false;
	output.load(img);
	return true;
}
