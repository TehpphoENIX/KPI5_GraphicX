#pragma once
#include "Graphics.h"

class Camera
{
public:
	Camera(
		DirectX::XMFLOAT3 targPosition = {0,0,0},
		DirectX::XMFLOAT3 camPosition = {5,0,0},
		DirectX::XMFLOAT3 rotation = {0,0,0});
	
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void SetPositionCart(DirectX::XMFLOAT3 camPosition);
	void SetPositionSph(DirectX::XMFLOAT3 camPosition);
	void Reset() noexcept;
private:
	float x;float y;float z;//target position
	float cx;float cy;float cz;//camera position
	float roll;float pitch;float yaw;//camera rotation
};

