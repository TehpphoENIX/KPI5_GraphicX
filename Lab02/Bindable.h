#pragma once
#include "GraphicsResource.h"
#include <memory>
#include <string>
#include "ConditionalNoexcept.h"

class Drawable;

class Bindable : public GraphicsResource
{
public:
	virtual void Bind(Graphics& gfx) noxnd = 0;
	virtual void InitializeParentReference(const Drawable&) noexcept
	{}
	virtual ~Bindable() = default;
};
