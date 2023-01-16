#include "Sun.h"
#include "BindableBase.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Sun::Sun(Graphics& gfx, DirectX::XMFLOAT3 dimensions, DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, std::wstring texture, float scale) :
	Planet(gfx,dimensions,translation,rotation,texture,scale)
{
//
//	// model deformation transform (per instance, not stored as bind)
//	UpdateDimensions(dimensions);
//	UpdatePosition(translation);
//	UpdateRotation(rotation);
//
//	namespace dx = DirectX;
//
//
//	struct Vertex
//	{
//		dx::XMFLOAT3 pos;
//		dx::XMFLOAT3 n;
//		struct TextureCoords
//		{
//			float u;
//			float v;
//		} tex;
//	};
//
//	Assimp::Importer imp;
//	const auto pModel = imp.ReadFile("models/daSphere.obj",
//		aiProcess_Triangulate |
//		aiProcess_JoinIdenticalVertices
//	);
//	const auto pMesh = pModel->mMeshes[0];
//
//	std::vector<Vertex> vertices;
//	vertices.reserve(pMesh->mNumVertices);
//	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
//	{
//		vertices.push_back({
//			{ pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale }
//			,*reinterpret_cast<dx::XMFLOAT3*>(&pMesh->mNormals[i])
//			,*reinterpret_cast<Vertex::TextureCoords*>(&pMesh->mTextureCoords[0][i])
//			});
//	}
//	float max = 0.0f;
//	for (auto var : vertices)
//	{
//		if (var.pos.x > max)
//		{
//			max = var.pos.x;
//		}
//	}
//
//	std::vector<unsigned short> indices;
//	indices.reserve(pMesh->mNumFaces * 3);
//	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
//	{
//		const auto& face = pMesh->mFaces[i];
//		assert(face.mNumIndices == 3);
//		indices.push_back(face.mIndices[0]);
//		indices.push_back(face.mIndices[1]);
//		indices.push_back(face.mIndices[2]);
//	}
//
//	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
//
//	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));
//
//	AddTextureBinds(gfx);
//
//	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
//
//	//Texture
//	AddBind(std::make_unique<Texture>(gfx, texture));
//	//Sampler
//	AddBind(std::make_unique<Sampler>(gfx));
//
//	//struct PSMaterialConstant
//	//{
//	//	DirectX::XMFLOAT3 color;
//	//	float specularIntensity = 0.6f;
//	//	float specularPower = 30.0f;
//	//	float padding[3];
//	//} pmc;
//	//pmc.color = material;
//	//AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc));
//
//
//	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
	AddTextureBinds(gfx);
}

void Sun::AddTextureBinds(Graphics & gfx)
{
	int i = 0;

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
}
