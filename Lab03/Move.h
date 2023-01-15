#pragma once
#include "Transformation.h"


class Move :
	public Transformation
{
private:
	DirectX::XMFLOAT3 speedVector;
public:
	Move(float dx, float dy, float dz);

	void updateSpeed(float dx, float dy, float dz);

	DirectX::XMFLOAT3 updatePosition(DirectX::XMFLOAT3 currentPosition, float time) override;
	DirectX::XMFLOAT3 updateRotation(DirectX::XMFLOAT3 currentRotation, float time) override;
};

