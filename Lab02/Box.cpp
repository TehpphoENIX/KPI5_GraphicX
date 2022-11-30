#include "Box.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "IndexedTriangleList.h"
#include "MyMath.h"


Box::Box(Graphics& gfx,
	DirectX::XMFLOAT3 dimensions,
	DirectX::XMFLOAT3 translation,
	DirectX::XMFLOAT3 rotation,
	DirectX::XMFLOAT3 speed,
	DirectX::XMFLOAT3 angularSpeed)
{
	// model deformation transform (per instance, not stored as bind)
	UpdateDimensions(dimensions);
	UpdatePosition(translation);
	UpdateRotation(rotation);
	UpdateSpeed(speed);
	UpdateAngularSpeed(angularSpeed);

	namespace dx = DirectX;

	struct Vertex
	{
		dx::XMFLOAT3 pos;
	};
	constexpr float side = 1.0f / 2.0f;

	std::vector<dx::XMFLOAT3> vertices;
	vertices.emplace_back(-side, -side, -side); // 0
	vertices.emplace_back(side, -side, -side); // 1
	vertices.emplace_back(-side, side, -side); // 2
	vertices.emplace_back(side, side, -side); // 3
	vertices.emplace_back(-side, -side, side); // 4
	vertices.emplace_back(side, -side, side); // 5
	vertices.emplace_back(-side, side, side); // 6
	vertices.emplace_back(side, side, side); // 7

	std::vector<Vertex> verts(vertices.size());
	for (size_t i = 0; i < vertices.size(); i++)
	{
		verts[i].pos = vertices[i];
	}
	const auto model = IndexedTriangleList(
		std::move(verts), {
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		}
	);

	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"ColorIndexVS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"ColorIndexPS.cso"));

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	struct PixelShaderConstants
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[8];
	};
	const PixelShaderConstants cb2 =
	{
		{
			{ 1.0f,1.0f,1.0f },
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 1.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f },
			{ 1.0f,0.0f,1.0f },
			{ 0.0f,1.0f,1.0f },
			{ 0.0f,0.0f,0.0f },
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Box::Update(float dt) noexcept
{
	roll = NormalizeAngle(roll + droll * dt);
	pitch = NormalizeAngle(pitch + dpitch * dt);
	yaw = NormalizeAngle(yaw + dyaw * dt);
	x += dx * dt;
	y += dy * dt;
	z += dz * dt;
}

void Box::UpdateDimensions(DirectX::XMFLOAT3 dimensions)
{
	DirectX::XMStoreFloat3x3(
		&mt,
		DirectX::XMMatrixScaling(dimensions.x, dimensions.y, dimensions.z)
	);
}

void Box::UpdatePosition(DirectX::XMFLOAT3 translation)
{
	x = translation.x;
	y = translation.y;
	z = translation.z;
}

void Box::UpdateRotation(DirectX::XMFLOAT3 rotation)
{
	roll = rotation.x;
	pitch = rotation.y;
	yaw = rotation.z;
}

void Box::UpdateSpeed(DirectX::XMFLOAT3 speed)
{
	dx = speed.x; 
	dy=speed.y; 
	dz=speed.z;
}

void Box::UpdateAngularSpeed(DirectX::XMFLOAT3 angularSpeed)
{
	droll = angularSpeed.x;
	dpitch = angularSpeed.y;
	dyaw = angularSpeed.z;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(x, y, z);
}