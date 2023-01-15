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
		struct
		{
			float u;
			float v;
		} tex;
	};

	constexpr float side = 1.0f / 2.0f;

	std::vector<Vertex> vertices(14);

	vertices[0].pos = { -side,-side,-side };
	vertices[0].tex = { 2.0f / 3.0f,0.0f / 4.0f };
	vertices[1].pos = { side,-side,-side };
	vertices[1].tex = { 1.0f / 3.0f,0.0f / 4.0f };
	vertices[2].pos = { -side,side,-side };
	vertices[2].tex = { 2.0f / 3.0f,1.0f / 4.0f };
	vertices[3].pos = { side,side,-side };
	vertices[3].tex = { 1.0f / 3.0f,1.0f / 4.0f };
	vertices[4].pos = { -side,-side,side };
	vertices[4].tex = { 2.0f / 3.0f,3.0f / 4.0f };
	vertices[5].pos = { side,-side,side };
	vertices[5].tex = { 1.0f / 3.0f,3.0f / 4.0f };
	vertices[6].pos = { -side,side,side };
	vertices[6].tex = { 2.0f / 3.0f,2.0f / 4.0f };
	vertices[7].pos = { side,side,side };
	vertices[7].tex = { 1.0f / 3.0f,2.0f / 4.0f };
	vertices[8].pos = { -side,-side,-side };
	vertices[8].tex = { 2.0f / 3.0f,4.0f / 4.0f };
	vertices[9].pos = { side,-side,-side };
	vertices[9].tex = { 1.0f / 3.0f,4.0f / 4.0f };
	vertices[10].pos = { -side,-side,-side };
	vertices[10].tex = { 3.0f / 3.0f,1.0f / 4.0f };
	vertices[11].pos = { -side,-side,side };
	vertices[11].tex = { 3.0f / 3.0f,2.0f / 4.0f };
	vertices[12].pos = { side,-side,-side };
	vertices[12].tex = { 0.0f / 3.0f,1.0f / 4.0f };
	vertices[13].pos = { side,-side,side };
	vertices[13].tex = { 0.0f / 3.0f,2.0f / 4.0f };

	const IndexedTriangleList model(
		std::move(vertices),{
			0,2,1,   2,3,1,
			4,8,5,   5,8,9,
			2,6,3,   3,6,7,
			4,5,7,   4,7,6,
			2,10,11, 2,11,6,
			12,3,7,  12,7,13
		}
	);
	//VertexBuffer
	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
	//VertexShader
	auto pvs = std::make_unique<VertexShader>(gfx, L"TexturedVS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));
	//Texture
	AddBind(std::make_unique<Texture>(gfx, L"Textures\\Cube.png"));
	//Sampler
	AddBind(std::make_unique<Sampler>(gfx));
	//PixelShader
	AddBind(std::make_unique<PixelShader>(gfx, L"TexturedPS.cso"));
	//IndexBuffer
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	/*struct PixelShaderConstants
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
	AddBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));*/
	//InputLayout
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
	//Topology
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	//TransformBuffer
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