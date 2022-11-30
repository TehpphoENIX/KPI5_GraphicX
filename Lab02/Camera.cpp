#include "Camera.h"
#include "imgui/imgui.h"
#include "MyMath.h"

namespace dx = DirectX;

Camera::Camera(DirectX::XMFLOAT3 targPosition, DirectX::XMFLOAT3 camPosition, DirectX::XMFLOAT3 rotation) :
	x(targPosition.x), y(targPosition.y), z(targPosition.z),
	roll(rotation.x), pitch(rotation.y), yaw(rotation.z),
	cx(camPosition.x), cy(camPosition.y), cz(camPosition.z)
{}

//normal get matrix
DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	return dx::XMMatrixLookAtLH(
		dx::XMVectorSet(cx, cy, cz, 0.0f), 
		dx::XMVectorSet(x, y, z, 0.0f),
		dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)
	) * dx::XMMatrixRotationRollPitchYaw(
		pitch, -yaw, roll
	);
}

void Camera::SpawnControlWindow() noexcept
{
	float r, theta, phi;
	r = sqrt(pow(cx, 2) + pow(cy, 2) + pow(cz, 2));
	float length = sqrt(pow(cx, 2) + pow(cy, 2));
	phi = sgn(cy)*acos(cx/length);
	theta = acos(cz / r);

	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Camera Position");
		ImGui::Text("X:%.2f", cx);
		ImGui::Text("Y:%.2f", cy);
		ImGui::Text("Z:%.2f", cz);
		ImGui::Text("R:%.2f", r);
		ImGui::Text("Phi:%.2f", phi);
		ImGui::Text("Theta:%.2f", theta);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::SetPositionCart(DirectX::XMFLOAT3 camPosition)
{
	cx = camPosition.x;
	cy = camPosition.y;
	cz = camPosition.z;
}

void Camera::SetPositionSph(DirectX::XMFLOAT3 camPosition)
{
	if (camPosition.z > M_PI / 2)
		camPosition.z = M_PI / 2;
	else if (camPosition.z < -M_PI / 2)
		camPosition.z = -M_PI / 2;
	cx = camPosition.x * cos(camPosition.z) * cos(camPosition.y);
	cy = camPosition.x * cos(camPosition.z) * sin(camPosition.y);
	cz = camPosition.x * sin(camPosition.z);
}

void Camera::Reset() noexcept
{
	cx = 5.0f;
	cy = 0.0f;
	cz = 0.0f;
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

