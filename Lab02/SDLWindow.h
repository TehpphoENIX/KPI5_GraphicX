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
public:
	class Exception : public ChiliException
	{
		using ChiliException::ChiliException;
	};
	class SDL_Exception : public Exception
	{
	public:
		SDL_Exception(int line, const char* file);
		const char* what() const noexcept override;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};
	class HrException : public Exception
	{
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
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
	Graphics& Gfx();
	Uint32 getID() { return SDL_GetWindowID(window); }
};

// error exception helper macro
#define SDL_EXEPT throw Window::SDL_Exception(__LINE__,__FILE__)
#define CHWND_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,(hr) )
#define CHWND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )
#define CHWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )