#include <SDL2/SDL.h>
#include "game.hpp"
#include <iostream>

int main()
{
	// Start SDL
	if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) 
	{
		std::cout << "SDL Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	// Run the game
	Game::run();
	
	// Stop SDL
	SDL_Quit();
	return 0;
}
