#pragma once
#include "Graphics.h"
#include "Sun.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity = 0;
		float attConst = 0;
		float attLin = 0;
		float attQuad = 0;
	};
private:
	PointLightCBuf cbData;
	mutable Sun mesh;
public:
	PointLight( Graphics& gfx, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, std::wstring texture = L"Textures/sun.jpg", float scale = 0.012f);
	void SpawnControlWindow() noexcept;
	void Reset(DirectX::XMFLOAT3 position = {0,0,0}) noexcept;
	void Draw( Graphics& gfx ) const noexcept(!IS_DEBUG);
	//void Bind( Graphics& gfx,DirectX::FXMMATRIX view ) const noexcept;
	PointLightCBuf& getData() { return cbData; }

};