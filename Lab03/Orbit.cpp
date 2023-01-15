#include "Orbit.h"
#include "MyMath.h"

Orbit::Orbit(DirectX::XMFLOAT3 centerOfRotation, float angularSpeed) :
    centerOfRotation(centerOfRotation), angularSpeed(angularSpeed)
{
}

DirectX::XMFLOAT3 Orbit::updatePosition(DirectX::XMFLOAT3 currentPosition, float time)
{
    DirectX::XMVECTOR cOR = DirectX::XMLoadFloat3(&centerOfRotation),
        cP = DirectX::XMLoadFloat3(&currentPosition),
        rV = DirectX::XMVectorSubtract(cP, cOR);
    float r, theta, phi;
    r = sqrt(pow(DirectX::XMVectorGetX(rV), 2) + pow(DirectX::XMVectorGetY(rV), 2) + pow(DirectX::XMVectorGetZ(rV), 2));
    float length = sqrt(pow(DirectX::XMVectorGetX(rV), 2) + pow(DirectX::XMVectorGetY(rV), 2));
    phi = sgn(DirectX::XMVectorGetY(rV)) * acos(DirectX::XMVectorGetX(rV) / length);
    theta = acos(DirectX::XMVectorGetZ(rV) / r);

    phi += angularSpeed;

    DirectX::XMFLOAT3 out = {0,0,0};

    out.x = r * sin(theta) * cos(phi) + centerOfRotation.x;
    out.y = r * sin(theta) * sin(phi) + centerOfRotation.y;
    out.z = r * cos(theta) + centerOfRotation.z;
    return out;
}

DirectX::XMFLOAT3 Orbit::updateRotation(DirectX::XMFLOAT3 currentRotation, float time)
{
    return currentRotation;
}
