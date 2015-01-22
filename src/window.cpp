#include "window.hpp"
#include <SDL2/SDL.h>

static SDL_Window* window = 0;
static SDL_GLContext context = 0;

void Window::open(int w, int h)
{
	// Open our window
	window = SDL_CreateWindow("Insert Game Name", 
														SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
														w, h, 
														SDL_WINDOW_OPENGL |
														SDL_WINDOW_SHOWN |
														SDL_WINDOW_RESIZABLE);
	
	// Create the opengl context
	context = SDL_GL_CreateContext(window);
}

void Window::close()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	context = 0;
	window = 0;
}

void Window::swap()
{
	SDL_GL_SwapWindow(window);
}

int Window::getWidth()
{
	int width = 0;
	SDL_GetWindowSize(window, &width, 0);
	return width;
}

int Window::getHeight()
{
	int height = 0;
	SDL_GetWindowSize(window, 0, &height);
	return height;
}
