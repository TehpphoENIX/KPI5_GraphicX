#include "MyMath.h"
#include "Rotate.h"

Rotate::Rotate(float droll, float dpitch, float dyaw)
{
	updateRotationSpeed(droll, dpitch, dyaw);
}

void Rotate::updateRotationSpeed(float droll, float dpitch, float dyaw)
{
	rotationSpeed.x = droll;
	rotationSpeed.y = dpitch;
	rotationSpeed.z = dyaw;
}

DirectX::XMFLOAT3 Rotate::updatePosition(DirectX::XMFLOAT3 currentPosition, float time)
{
	return currentPosition;
}

DirectX::XMFLOAT3 Rotate::updateRotation(DirectX::XMFLOAT3 currentRotation, float time)
{
	DirectX::XMFLOAT3 out;

	out.x = NormalizeAngle(currentRotation.x + rotationSpeed.x * time);
	out.y = NormalizeAngle(currentRotation.y + rotationSpeed.y * time);
	out.z = NormalizeAngle(currentRotation.z + rotationSpeed.z * time);

	return out;
}
