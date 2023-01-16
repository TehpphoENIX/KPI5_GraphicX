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
		/*dx::XMFLOAT3 normal;
		struct
		{
			float u;
			float v;
		} tex;*/
	};

	constexpr float side = 150.0f / 2.0f;

	std::vector<Vertex> vertices(14);

	vertices[0].pos = { -side,-side,-side };
	vertices[1].pos = { side,-side,-side };
	vertices[2].pos = { -side,side,-side };
	vertices[3].pos = { side,side,-side };
	vertices[4].pos = { -side,-side,side };
	vertices[5].pos = { side,-side,side };
	vertices[6].pos = { -side,side,side };
	vertices[7].pos = { side,side,side };

	const IndexedTriangleList model(
		std::move(vertices),{
			1,2,0, 1,3,2,
			5,3,1, 5,7,3,
			3,6,2, 7,6,3,
			7,5,4, 6,7,4,
			2,4,0, 6,4,2,
			4,1,0, 4,5,1
		}
	);
	//VertexBuffer
	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
	//VertexShader
	auto pvs = std::make_unique<VertexShader>(gfx, L"Skybox_VS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));
	//Texture
	AddBind(std::make_unique<CubeTexture>(gfx, "Textures/StarsBox"));
	//Sampler
	AddBind(std::make_unique<Sampler>(gfx));
	//PixelShader
	AddBind(std::make_unique<PixelShader>(gfx, L"Skybox_PS.cso"));
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
		//{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
	//Topology
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	//TransformBuffer
	AddBind(std::make_unique<SkyboxTransformCbuf>(gfx));
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