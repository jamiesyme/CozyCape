#pragma once

#include <string>
#include "image.hpp"
#include "texture.hpp"

namespace FontHelper {
	void init();
	void kill();
	
	bool setAlias(const std::string& filename,
	              const std::string& alias);
	              
	bool render(const std::string& text, 
	            const std::string& fontName, 
	            const int size,
	            const Color& color,
	            Image& output);
	bool render(const std::string& text, 
	            const std::string& fontName, 
	            const int size,
	            const Color& color,
	            Texture& output);
};
