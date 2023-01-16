#pragma once
#include "Planet.h"

class Sun :
    public Planet
{
public:
	Sun(Graphics& gfx,
		DirectX::XMFLOAT3 dimensions,
		DirectX::XMFLOAT3 translation,
		DirectX::XMFLOAT3 rotation,
		std::wstring texture,
		float scale = 1.0f);
protected:
	void AddTextureBinds(Graphics& gfx) override;
};

