#include "SDLWindow.h"
#include <SDL_syswm.h>
#include <imgui_impl_sdl.h>

Window::Window(std::string name, std::pair<int, int> window_size, SDL_WindowFlags window_flags, std::pair<int, int> window_position):
	window(SDL_CreateWindow(name.c_str(), window_position.first, window_position.second, window_size.first, window_size.second, window_flags))
{
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
	hwnd = (HWND)wmInfo.info.win.window;
	//
	ImGui_ImplSDL2_InitForD3D(window);
	//make graphics object
	pGfx = std::make_unique<Graphics>(hwnd, 0, 0);

}

Window::~Window()
{
	SDL_DestroyWindow(window);
}
