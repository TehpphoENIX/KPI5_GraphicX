#include "Planet.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "IndexedTriangleList.h"
#include "MyMath.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Planet::Planet(Graphics& gfx,
	DirectX::XMFLOAT3 dimensions,
	DirectX::XMFLOAT3 translation,
	DirectX::XMFLOAT3 rotation, std::wstring texture, float scale)
{
	// model deformation transform (per instance, not stored as bind)
	UpdateDimensions(dimensions);
	UpdatePosition(translation);
	UpdateRotation(rotation);

	namespace dx = DirectX;

	
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
			struct TextureCoords
			{
				float u;
				float v;
			} tex;
		};

		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("models/daSphere.obj",
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
				,*reinterpret_cast<Vertex::TextureCoords*>(&pMesh->mTextureCoords[0][i])
				});
		}
		float max = 0.0f;
		for (auto var : vertices)
		{
			if (var.pos.x > max)
			{
				max = var.pos.x;
			}
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

		auto pvs = std::make_unique<VertexShader>(gfx, L"TexturedVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));

		AddBind(std::make_unique<PixelShader>(gfx, L"TexturedPS.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		//Texture
		AddBind(std::make_unique<Texture>(gfx, texture));
		//Sampler
		AddBind(std::make_unique<Sampler>(gfx));

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

void Planet::Update(float dt) noexcept
{
	for (auto& var : transformationStack) 
	{
		position = var->updatePosition(position, dt);
		rotation = var->updateRotation(rotation, dt);
	}
}

void Planet::UpdateDimensions(DirectX::XMFLOAT3 dimensions)
{
	DirectX::XMStoreFloat3x3(
		&mt,
		DirectX::XMMatrixScaling(dimensions.x, dimensions.y, dimensions.z) 
		* DirectX::XMMatrixRotationRollPitchYaw(-M_PI/2,0,0)
	);
}

void Planet::UpdatePosition(DirectX::XMFLOAT3 translation)
{
	position.x = translation.x;
	position.y = translation.y;
	position.z = translation.z;
}

void Planet::UpdateRotation(DirectX::XMFLOAT3 rotation)
{
	rotation.x = rotation.x;
	rotation.y = rotation.y;
	rotation.z = rotation.z;
}

DirectX::XMMATRIX Planet::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		dx::XMMatrixTranslation(position.x, position.y, position.z);
}