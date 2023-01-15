#pragma once
#include <DirectXMath.h>

class Transformation
{
public:
	virtual DirectX::XMFLOAT3 updatePosition(DirectX::XMFLOAT3 currentPosition, float time) = 0;
	virtual DirectX::XMFLOAT3 updateRotation(DirectX::XMFLOAT3 currentRotation, float time) = 0;
	virtual ~Transformation() { };
};