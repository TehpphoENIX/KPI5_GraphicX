#pragma once
#include "Transformation.h"
class Orbit :
    public Transformation
{
private:
    DirectX::XMFLOAT3 centerOfRotation;
    float angularSpeed;
public:
    Orbit(DirectX::XMFLOAT3 centerOfRotation, float angularSpeed);

    DirectX::XMFLOAT3 updatePosition(DirectX::XMFLOAT3 currentPosition, float time) override;
    DirectX::XMFLOAT3 updateRotation(DirectX::XMFLOAT3 currentRotation, float time) override;
};

