#pragma once
#include "Transformation.h"
class Rotate :
    public Transformation
{
private:
    DirectX::XMFLOAT3 rotationSpeed;
public:
    Rotate(float droll, float dpitch, float dyaw);

    void updateRotationSpeed(float droll, float dpitch, float dyaw);

    DirectX::XMFLOAT3 updatePosition(DirectX::XMFLOAT3 currentPosition, float time) override;
    DirectX::XMFLOAT3 updateRotation(DirectX::XMFLOAT3 currentRotation, float time) override;
};
