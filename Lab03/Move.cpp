#include "Move.h"

Move::Move(float dx, float dy, float dz)
{
	updateSpeed(dx, dy, dz);
}

void Move::updateSpeed(float dx, float dy, float dz)
{
	speedVector.x = dx;
	speedVector.y = dy;
	speedVector.z = dz;
}

DirectX::XMFLOAT3 Move::updatePosition(DirectX::XMFLOAT3 currentPosition, float time)
{
	DirectX::XMFLOAT3 out;

	out.x = currentPosition.x + speedVector.x * time;
	out.y = currentPosition.y + speedVector.y * time;
	out.z = currentPosition.z + speedVector.z * time;

	return out;
}

DirectX::XMFLOAT3 Move::updateRotation(DirectX::XMFLOAT3 currentRotation, float time)
{
	return currentRotation;
}
