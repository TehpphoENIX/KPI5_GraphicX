#include "Torus.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "IndexedTriangleList.h"
#include "MyMath.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Torus::Torus(Graphics& gfx,
	DirectX::XMFLOAT3 dimensions,
	DirectX::XMFLOAT3 translation,
	DirectX::XMFLOAT3 rotation,
	DirectX::XMFLOAT3 speed,
	DirectX::XMFLOAT3 angularSpeed,
	DirectX::XMFLOAT3 material, float scale)
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
			dx::XMFLOAT3 n;
		};

		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("models\\Torus.obj",
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);
		const auto pMesh = pModel->mMeshes[0];

		std::vector<Vertex> vertices;
		vertices.reserve(pMesh->mNumVertices);
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertices.push_back({
				{ pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale }
				,*reinterpret_cast<dx::XMFLOAT3*>(&pMesh->mNormals[i])
				});
		}

		std::vector<unsigned short> indices;
		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

		AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"ColorIndexVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));

		AddBind(std::make_unique<PixelShader>(gfx, L"ColorIndexPS.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

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

		AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		//struct PSMaterialConstant
		//{
		//	DirectX::XMFLOAT3 color;
		//	float specularIntensity = 0.6f;
		//	float specularPower = 30.0f;
		//	float padding[3];
		//} pmc;
		//pmc.color = material;
		//AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc));
	

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Torus::Update(float dt) noexcept
{
	roll = NormalizeAngle(roll + droll * dt);
	pitch = NormalizeAngle(pitch + dpitch * dt);
	yaw = NormalizeAngle(yaw + dyaw * dt);
	x += dx * dt;
	y += dy * dt;
	z += dz * dt;
}

void Torus::UpdateDimensions(DirectX::XMFLOAT3 dimensions)
{
	DirectX::XMStoreFloat3x3(
		&mt,
		DirectX::XMMatrixScaling(dimensions.x, dimensions.y, dimensions.z) 
		* DirectX::XMMatrixRotationRollPitchYaw(-M_PI/2,0,0)
	);
}

void Torus::UpdatePosition(DirectX::XMFLOAT3 translation)
{
	x = translation.x;
	y = translation.y;
	z = translation.z;
}

void Torus::UpdateRotation(DirectX::XMFLOAT3 rotation)
{
	roll = rotation.x;
	pitch = rotation.y;
	yaw = rotation.z;
}

void Torus::UpdateSpeed(DirectX::XMFLOAT3 speed)
{
	dx = speed.x;
	dy = speed.y;
	dz = speed.z;
}

void Torus::UpdateAngularSpeed(DirectX::XMFLOAT3 angularSpeed)
{
	droll = angularSpeed.x;
	dpitch = angularSpeed.y;
	dyaw = angularSpeed.z;
}

DirectX::XMMATRIX Torus::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(x, y, z);
}