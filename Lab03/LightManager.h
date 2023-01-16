#pragma once
#include "PointLight.h"
#include <array>
#define NUMBER_OF_LIGHTS 5

class LightManager
{
private:
	std::array<std::unique_ptr<PointLight>, NUMBER_OF_LIGHTS> lights;
	UINT size = 0;
	struct wtfBox 
	{
		PointLight::PointLightCBuf items[NUMBER_OF_LIGHTS];
	};
	mutable PixelConstantBuffer<wtfBox> cbuf;
public:
	LightManager(Graphics& gfx, UINT slot = 1U);

	void Add(std::unique_ptr<PointLight> light);
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view);
	void Draw(Graphics& gfx);
	PointLight* Get(UINT index) { return lights[index].get(); }
};

