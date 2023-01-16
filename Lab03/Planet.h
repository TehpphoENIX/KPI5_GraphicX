#pragma once
#include "Drawable.h"
#include <DirectXMath.h>
#include "Transformation.h"

class Planet : public Drawable
{
public:
	Planet(Graphics& gfx,
		DirectX::XMFLOAT3 dimensions,
		DirectX::XMFLOAT3 translation,
		DirectX::XMFLOAT3 rotation,
		std::wstring texture,
		float scale = 1.0f);
	Planet() {}
	std::vector<std::shared_ptr<Transformation>>& getTransformation() 
	{
		return transformationStack;
	}
	void setTransformation(std::vector<std::shared_ptr<Transformation>> transformationStack) 
	{
		this->transformationStack = transformationStack;
	}
	void addTransformation(std::shared_ptr<Transformation> transformation)
	{
		transformationStack.push_back(transformation);
	}

	void Update(float dt) noexcept override;
	void UpdateDimensions(DirectX::XMFLOAT3 dimensions);
	void UpdatePosition(DirectX::XMFLOAT3 translation);
	void UpdateRotation(DirectX::XMFLOAT3 rotation);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
protected:
	// positional
	DirectX::XMFLOAT3 position;

	DirectX::XMFLOAT3 rotation;

	std::vector<std::shared_ptr<Transformation>> transformationStack;
	// model transform
	DirectX::XMFLOAT3X3 mt;

	virtual void AddTextureBinds(Graphics& gfx);

	void SyncMaterial(Graphics& gfx) noexcept(!IS_DEBUG);
	struct PSMaterialConstant
	{
		float specularIntensity;
		float specularPower;
		float padding[2];
	} materialConstants;
	//using MaterialCbuf = PixelConstantBuffer<PSMaterialConstant>;
};

