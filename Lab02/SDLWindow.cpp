#include "SDLWindow.h"
#include <SDL_syswm.h>
#include "ImGUI/imgui_impl_sdl.h"
#include <sstream>
#include <locale>
#include <utility>
#include <codecvt>
#include <stringapiset.h>

Window::Window(std::string name, std::pair<int, int> window_size, SDL_WindowFlags window_flags, std::pair<int, int> window_position):
	window(SDL_CreateWindow(name.c_str(), window_position.first, window_position.second, window_size.first, window_size.second, window_flags))
{
	if (window == NULL) SDL_EXEPT;
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	if (SDL_GetWindowWMInfo(window, &wmInfo) == SDL_FALSE) SDL_EXEPT;
	hwnd = (HWND)wmInfo.info.win.window;
	//init ImGui platform context
	ImGui_ImplSDL2_InitForD3D(window);
	//make graphics object
	pGfx = std::make_unique<Graphics>(hwnd, window_size.first, window_size.second);

}

Window::~Window()
{
	SDL_DestroyWindow(window);
}

Graphics& Window::Gfx()
{
	if (!pGfx)
	{
		throw CHWND_NOGFX_EXCEPT();
	}
	return *pGfx;
}

// Window Exception Stuff
std::string Window::HrException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

Window::SDL_Exception::SDL_Exception(int line, const char* file):
	Exception(line, file)
{}

const char* Window::SDL_Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << "SDL error:\n" << SDL_GetError() << std::endl
		<< GetOriginString();
	SDL_ClearError();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::NoGfxException::GetType() const noexcept
{
	return "Chili Window Exception [No Graphics]";
}

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Chili Graphics Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
	return HrException::TranslateErrorCode(hr);
}