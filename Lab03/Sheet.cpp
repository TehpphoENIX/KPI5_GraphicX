#include "Sheet.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Texture.h"
#include "Sampler.h"
#include "MyMath.h"
#include <memory>
#include "IndexedTriangleList.h"

Sheet::Sheet(Graphics& gfx,
	DirectX::XMFLOAT3 dimensions,
	DirectX::XMFLOAT3 translation,
	DirectX::XMFLOAT3 rotation,
	DirectX::XMFLOAT3 speed,
	DirectX::XMFLOAT3 angularSpeed)
{
	UpdateDimensions(dimensions);
	UpdatePosition(translation);
	UpdateRotation(rotation);
	UpdateSpeed(speed);
	UpdateAngularSpeed(angularSpeed);

	namespace dx = DirectX;

	struct Vertex
	{
		dx::XMFLOAT3 pos;
		struct
		{
			float u;
			float v;
		} tex;
	};
	constexpr float side = 1.0f / 2.0f;
	std::vector<dx::XMFLOAT3> vertices;
	vertices.emplace_back(-side, -side, 0); // 0
	vertices.emplace_back(side, -side, 0); // 1
	vertices.emplace_back(-side, side, 0); // 2
	vertices.emplace_back(side, side, 0); // 3

	std::vector<Vertex> verts(vertices.size());
	for (size_t i = 0; i < vertices.size(); i++)
	{
		verts[i].pos = vertices[i];
	}
	const auto model = IndexedTriangleList(
		std::move(verts), {
			0,2,1, 2,3,1,
		}
	);

	AddBind(std::make_unique<Texture>(gfx, L"Textures\\Checker.jpg"));

	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	AddBind(std::make_unique<Sampler>(gfx));

	auto pvs = std::make_unique<VertexShader>(gfx, L"TextureVS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"TexturePS.cso"));

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Sheet::Update(float dt) noexcept
{
	roll = NormalizeAngle(roll + droll * dt);
	pitch = NormalizeAngle(pitch + dpitch * dt);
	yaw = NormalizeAngle(yaw + dyaw * dt);
	x += dx * dt;
	y += dy * dt;
	z += dz * dt;
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(x, y, z);
}

void Sheet::UpdateDimensions(DirectX::XMFLOAT3 dimensions)
{
	DirectX::XMStoreFloat3x3(
		&mt,
		DirectX::XMMatrixScaling(dimensions.x, dimensions.y, dimensions.z)
	);
}

void Sheet::UpdatePosition(DirectX::XMFLOAT3 translation)
{
	x = translation.x;
	y = translation.y;
	z = translation.z;
}

void Sheet::UpdateRotation(DirectX::XMFLOAT3 rotation)
{
	roll = rotation.x;
	pitch = rotation.y;
	yaw = rotation.z;
}

void Sheet::UpdateSpeed(DirectX::XMFLOAT3 speed)
{
	dx = speed.x;
	dy = speed.y;
	dz = speed.z;
}

void Sheet::UpdateAngularSpeed(DirectX::XMFLOAT3 angularSpeed)
{
	droll = angularSpeed.x;
	dpitch = angularSpeed.y;
	dyaw = angularSpeed.z;
}
