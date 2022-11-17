#include "Window.h"

int CALLBACK WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	Window wnd(640, 480, L"MyWindow");
	 
	MSG msg;
	BOOL gResult;
	while (gResult=GetMessageW(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	if (gResult == -1) return -1;
	else return msg.wParam;
}