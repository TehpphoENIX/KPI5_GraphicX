#include "LightManager.h"

LightManager::LightManager(Graphics& gfx, UINT slot) : 
	cbuf(gfx,slot)
{
}

void LightManager::Add(std::unique_ptr<PointLight> light)
{
	if (size < NUMBER_OF_LIGHTS)
	{
		lights[size] = std::move(light);
		size++;
	}
}

void LightManager::Bind(Graphics& gfx, DirectX::FXMMATRIX view)
{
	wtfBox wtf;
	for (UINT i = 0; i < size; i++)
	{
		wtf.items[i] = lights[i]->getData();
		const auto pos = DirectX::XMLoadFloat3(&lights[i]->getData().pos);
		DirectX::XMStoreFloat3(&wtf.items[i].pos, DirectX::XMVector3Transform(pos, view));
	}
	cbuf.Update(gfx, wtf);
	cbuf.Bind(gfx);
}

void LightManager::Draw(Graphics& gfx)
{
	for (size_t i = 0; i < size; i++)
	{
		lights[i]->Draw(gfx);
	}
}
