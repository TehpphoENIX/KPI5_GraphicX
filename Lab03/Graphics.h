#pragma once
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

#include "fixMyWin.h"
#include "ImGUI/imgui.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <vector>
#include "DxgiInfoManager.h"
#include "ChiliException.h"
#include <DirectXMath.h>

//const DirectX::XMFLOAT2 TestObjectVertices[] =
//{
//	/*A 0*/{+0.0f,+0.697f},/*B 1*/{+0.354f,+0.697f},
//	/*C 2*/{-0.446f,+0.553f},/*D 3*/{-0.2f,+0.553f},/*E 4*/{+0.154f,+0.553f},
//	/*F 5*/{-0.446f,+0.316f},/*G 6*/{-0.2f,+0.316f},
//	/*H 7*/{-0.2f,+0.0f},
//	/*I 8*/{-0.345f,-0.105f},/*J 9*/{-0.2f,-0.105f},/*K 10*/{+0.323f,-0.105f},
//	/*L 11*/{-0.2f,-0.210f},
//	/*M 12*/{-0.2f,-0.342f},/*N 13*/{+0.081f,-0.342f},
//	/*O 14*/{-0.2f,-0.553f},/*P 15*/{+0.323f,-0.553f}
//};
//const unsigned short TestObjectIndicesTriangles[] =
//{
//	0,4,3,//
//	0,1,4,//EAR
//	2,3,6,//
//	2,6,5,//HEAD
//	6,10,9,//TORSO
//	7,11,8,//HAND
//	9,10,15,//
//	13,15,14,//
//	12,13,14//LEG
//};
//const unsigned short TestObjectIndicesLines[] =
//{
//	0,1,0,3,1,4,3,4,
//	2,3,3,6,6,5,5,2,
//	6,10,10,9,9,6,//TORSO
//	7,11,11,8,8,7,//HAND
//	9,10,10,15,15,9,//
//	13,15,15,14,14,13,//
//	12,13,13,14,14,12//LEG
//};

class Graphics
{
	friend class GraphicsResource;
public:
	class Exception : public ChiliException
	{
		using ChiliException::ChiliException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRSWireframe;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	int width;
	int height;
public:
	Graphics(HWND hWnd, int width=0, int height=0);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void EndFrame();
	void ClearBuffer(ImVec4 clear_color) noexcept;
	void DrawIndexed(UINT count) noexcept(!IS_DEBUG);

	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;

	void SetCamera(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;

	void wireframe();
	void solid();
	void perspective(float nearZ = 0.5f, float farZ = 20.f);
	void orthographic(float nearZ = 0.5f, float farZ = 20.f);

	//struct TestObject
	//{
	//	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	//	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBufferTriangles;
	//	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBufferLines;
	//	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	//	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	//	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	//
	//	UINT stride;
	//	UINT offset;
	//
	//	DirectX::XMFLOAT4 fillColor;
	//	DirectX::XMFLOAT4 outlineColor;
	//
	//} Rabbit;
	//void CreateTestObject(DirectX::XMFLOAT4 fillColor, DirectX::XMFLOAT4 outlineColor)
	//{
	//	//colors
	//	Rabbit.fillColor = fillColor;
	//	Rabbit.outlineColor = outlineColor;
	//	//vertex buffer
	//	D3D11_BUFFER_DESC bd = {};
	//	{
	//			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//			bd.Usage = D3D11_USAGE_IMMUTABLE;
	//			bd.CPUAccessFlags = 0u;
	//			bd.MiscFlags = 0u;
	//			bd.ByteWidth = sizeof(TestObjectVertices);
	//			bd.StructureByteStride = sizeof(DirectX::XMFLOAT2);
	//	}
	//	D3D11_SUBRESOURCE_DATA sd = {};
	//	{
	//		sd.pSysMem = TestObjectVertices;
	//	}
	//	pDevice->CreateBuffer(&bd, &sd, &Rabbit.pVertexBuffer);
	//	Rabbit.stride = sizeof(DirectX::XMFLOAT2);
	//	Rabbit.offset = 0u;
	//	//vertex shader
	//	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	//	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	//	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &Rabbit.pVertexShader);
	//	//input layout
	//	const D3D11_INPUT_ELEMENT_DESC ied[] = {
	//		{
	//			/*		  SemanticName:*/"POSITION",
	//			/*		 SemanticIndex:*/0,
	//			/*				Format:*/DXGI_FORMAT_R32G32_FLOAT,
	//			/*			 InputSlot:*/0,
	//			/*	AlingnedByteOffset:*/0,
	//			/*		InputSlotClass:*/D3D11_INPUT_PER_VERTEX_DATA,
	//			/*InstanceDataStepRate:*/0
	//		}
	//	};
	//	pDevice->CreateInputLayout(ied, 1, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &Rabbit.pInputLayout);
	//	//pixel shader
	//	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	//	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &Rabbit.pPixelShader);
	//	//index buffer
	//	D3D11_BUFFER_DESC ibd = {};
	//	{
	//		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//		ibd.Usage = D3D11_USAGE_IMMUTABLE;
	//		ibd.CPUAccessFlags = 0u;
	//		ibd.MiscFlags = 0u;
	//		ibd.ByteWidth = sizeof(TestObjectIndicesTriangles);
	//		ibd.StructureByteStride = sizeof(unsigned short);
	//	}
	//	D3D11_SUBRESOURCE_DATA isd = {};
	//	{
	//		isd.pSysMem = TestObjectIndicesTriangles;
	//	}
	//	pDevice->CreateBuffer(&ibd, &isd, &Rabbit.pIndexBufferTriangles);
	//	{
	//		ibd.ByteWidth = sizeof(TestObjectIndicesLines);
	//		isd.pSysMem = TestObjectIndicesLines;
	//	}
	//	pDevice->CreateBuffer(&ibd, &isd, &Rabbit.pIndexBufferLines);
	//}
	//void DrawTestObject(DirectX::XMFLOAT2 pos)
	//{
	//	struct ConstantBuffer {
	//		DirectX::XMMATRIX transform;
	//		DirectX::XMFLOAT4 color;
	//	};
	//
	//	ConstantBuffer cb =
	//	{
	//		DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(pos.x,pos.y,0.0f)),
	//		Rabbit.fillColor
	//	};
	//
	//	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	//	D3D11_BUFFER_DESC cbd = {};
	//	{
	//		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//		cbd.Usage = D3D11_USAGE_DYNAMIC;
	//		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//		cbd.MiscFlags = 0u;
	//		cbd.ByteWidth = sizeof(cb);
	//		cbd.StructureByteStride = 0u;
	//	}
	//	D3D11_SUBRESOURCE_DATA csd = {};
	//	{
	//		csd.pSysMem = &cb;
	//	}
	//	pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	//
	//	pContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
	//	pContext->PSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
	//	pContext->IASetVertexBuffers(0u, 1u, Rabbit.pVertexBuffer.GetAddressOf(), &Rabbit.stride, &Rabbit.offset);
	//	pContext->IASetInputLayout(Rabbit.pInputLayout.Get());
	//	pContext->VSSetShader(Rabbit.pVertexShader.Get(), nullptr, 0u);
	//	pContext->PSSetShader(Rabbit.pPixelShader.Get(), nullptr, 0u);
	//	D3D11_VIEWPORT vp;
	//	{
	//		vp.Width = width;
	//		vp.Height = height;
	//		vp.MinDepth = 0;
	//		vp.MaxDepth = 1;
	//		vp.TopLeftX = 0;
	//		vp.TopLeftY = 0;
	//	}
	//	pContext->RSSetViewports(1u, &vp);
	//	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	pContext->IASetIndexBuffer(Rabbit.pIndexBufferTriangles.Get(), DXGI_FORMAT_R16_UINT, 0u);
	//	pContext->DrawIndexed((UINT)std::size(TestObjectIndicesTriangles), 0u, 0u);
	//	cb.color = Rabbit.outlineColor;
	//	pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	//	pContext->PSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
	//	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//	pContext->IASetIndexBuffer(Rabbit.pIndexBufferLines.Get(), DXGI_FORMAT_R16_UINT, 0u);
	//	pContext->DrawIndexed((UINT)std::size(TestObjectIndicesLines), 0u, 0u);
	//}
};
