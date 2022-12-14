#include "GraphPlane.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "IndexedTriangleList.h"
#include "MyMath.h"


double fun(double x, double y)
{
	return sin(x) * cos(y);
}

GraphPlane::GraphPlane(Graphics& gfx,
	DirectX::XMFLOAT3 dimensions,
	DirectX::XMFLOAT3 translation,
	DirectX::XMFLOAT3 rotation,
	DirectX::XMFLOAT3 speed,
	DirectX::XMFLOAT3 angularSpeed,
	DirectX::XMFLOAT3 graphPos,
	float scale)
{
	// model deformation transform (per instance, not stored as bind)
	UpdateDimensions(dimensions);
	UpdatePosition(translation);
	UpdateRotation(rotation);
	UpdateSpeed(speed);
	UpdateAngularSpeed(angularSpeed);
	UpdateGraphPosition(graphPos);

	namespace dx = DirectX;

	struct Vertex
	{
		dx::XMFLOAT3 pos;
	};
	constexpr float side = 1.0f / 2.0f;
	const unsigned int gridsize = 100;
	const float delta = side * 2 / (gridsize-1);

	std::vector<dx::XMFLOAT3> vertices;
	for (int i = 0; i < gridsize; i++)
	{
		for (int j = 0; j < gridsize; j++)
		{
			float ix = - side + delta * j;
			float iy = - side + delta * i;
			vertices.emplace_back(ix, iy, fun(ix* scale, iy* scale));
		}
	}

	std::vector<unsigned short> indicies;
	for (int i = 1; i < gridsize; i++)
	{
		for (int j = 1; j < gridsize; j++)
		{
			indicies.emplace_back(gridsize* (i - 1) + j - 1);
			indicies.emplace_back(gridsize* (i - 1) + j);
			indicies.emplace_back(gridsize* i + j - 1);
			indicies.emplace_back(gridsize* i + j - 1);
			indicies.emplace_back(gridsize* (i - 1) + j);
			indicies.emplace_back(gridsize* i + j);
		}
	}

	std::vector<Vertex> verts(vertices.size());
	for (size_t i = 0; i < vertices.size(); i++)
	{
		verts[i].pos = vertices[i];
	}
	const auto model = IndexedTriangleList(
		std::move(verts), std::move(indicies)
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

void GraphPlane::Update(float dt) noexcept
{
	roll = NormalizeAngle(roll + droll * dt);
	pitch = NormalizeAngle(pitch + dpitch * dt);
	yaw = NormalizeAngle(yaw + dyaw * dt);
	x += dx * dt;
	y += dy * dt;
	z += dz * dt;
}

void GraphPlane::UpdateDimensions(DirectX::XMFLOAT3 dimensions)
{
	DirectX::XMStoreFloat3x3(
		&mt,
		DirectX::XMMatrixScaling(dimensions.x, dimensions.y, dimensions.z)
	);
}

void GraphPlane::UpdatePosition(DirectX::XMFLOAT3 translation)
{
	x = translation.x;
	y = translation.y;
	z = translation.z;
}

void GraphPlane::UpdateRotation(DirectX::XMFLOAT3 rotation)
{
	roll = rotation.x;
	pitch = rotation.y;
	yaw = rotation.z;
}

void GraphPlane::UpdateSpeed(DirectX::XMFLOAT3 speed)
{
	dx = speed.x;
	dy = speed.y;
	dz = speed.z;
}

void GraphPlane::UpdateAngularSpeed(DirectX::XMFLOAT3 angularSpeed)
{
	droll = angularSpeed.x;
	dpitch = angularSpeed.y;
	dyaw = angularSpeed.z;
}

void GraphPlane::UpdateGraphPosition(DirectX::XMFLOAT3 graphCenter)
{
	gx = graphCenter.x;
	gy = graphCenter.y;
	gz = graphCenter.z;
}

DirectX::XMMATRIX GraphPlane::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(x, y, z);
}