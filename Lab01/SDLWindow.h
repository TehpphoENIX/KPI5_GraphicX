#pragma once
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")

#include "fixMyWin.h"
#include <SDL.h>
#include <utility>
#include <string>
#include <memory>
#include "Graphics.h"

class Window
{
private:
	SDL_Window* window;
	HWND hwnd;
	std::unique_ptr<Graphics> pGfx;
public:
	Window(
		std::string name, 
		std::pair<int,int> window_size, 
		SDL_WindowFlags window_flags, 
		std::pair<int,int> window_position = { SDL_WINDOWPOS_CENTERED ,SDL_WINDOWPOS_CENTERED }
	);
	~Window();
	Graphics& Gfx() { return *pGfx; }
	Uint32 getID() { return SDL_GetWindowID(window); }
};

