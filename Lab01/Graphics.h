#pragma once
#include "fixMyWin.h"
#include <imgui.h>
#include <d3d11.h>
class Graphics
{
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
	int width;
	int height;
public:
	Graphics(HWND hWnd, int width=0, int height=0);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	void ClearBuffer(ImVec4 clear_color) noexcept
	{
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		pContext->OMSetRenderTargets(1, &pTarget, NULL);
		pContext->ClearRenderTargetView(pTarget, clear_color_with_alpha);
	}
};