#pragma once
#include "Drawable.h"
#include <DirectXMath.h>

class GraphPlane : public Drawable
{
public:
	GraphPlane(Graphics& gfx,
		DirectX::XMFLOAT3 dimensions,
		DirectX::XMFLOAT3 translation,
		DirectX::XMFLOAT3 rotation,
		DirectX::XMFLOAT3 speed = { 0.0f,0.0f,0.0f },
		DirectX::XMFLOAT3 angularSpeed = { 0.0f,0.0f,0.0f },
		DirectX::XMFLOAT3 graphPos = { 0.0f,0.0f,0.0f },
		float scale = 1.0f);
	void Update(float dt) noexcept override;
	void UpdateDimensions(DirectX::XMFLOAT3 dimensions);
	void UpdatePosition(DirectX::XMFLOAT3 translation);
	void UpdateRotation(DirectX::XMFLOAT3 rotation);
	void UpdateSpeed(DirectX::XMFLOAT3 speed = { 0.0f,0.0f,0.0f });
	void UpdateAngularSpeed(DirectX::XMFLOAT3 angularSpeed = { 0.0f,0.0f,0.0f });
	void UpdateGraphPosition(DirectX::XMFLOAT3 angularSpeed = { 0.0f,0.0f,0.0f });
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// positional
	float x;
	float y;
	float z;

	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;

	float dx;
	float dy;
	float dz;

	//graph
	float gx;
	float gy;
	float gz;

	// model transform
	DirectX::XMFLOAT3X3 mt;
};

