#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>

class SceneObject
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;

	DirectX::XMFLOAT2 vertices[];
public:
	SceneObject(
		ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		DirectX::XMFLOAT4 fillColor, DirectX::XMFLOAT4 outlineColor,
		std::wstring VertexShader, std::wstring Pixel
	);
	SceneObject(const SceneObject&) = delete;
	SceneObject& operator=(const SceneObject&) = delete;
	~SceneObject() = default;

	void Draw(DirectX::XMFLOAT2 coords);
};

