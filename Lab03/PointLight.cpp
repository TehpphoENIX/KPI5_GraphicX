#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, std::wstring texture, float scale)
	:
	mesh(gfx, { 1.0f,1.0f,1.0f }, position, rotation, texture, scale)
{
	Reset(position);
}

void PointLight::SpawnControlWindow() noexcept
{
	if( ImGui::Begin( "Light" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X",&cbData.pos.x,-60.0f,60.0f,"%.1f" );
		ImGui::SliderFloat( "Y",&cbData.pos.y,-60.0f,60.0f,"%.1f" );
		ImGui::SliderFloat( "Z",&cbData.pos.z,-60.0f,60.0f,"%.1f" );
		
		ImGui::Text( "Intensity/Color" );
		ImGui::SliderFloat( "Intensity",&cbData.diffuseIntensity,0.01f,2.0f,"%.2f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::ColorEdit3( "Diffuse Color",&cbData.diffuseColor.x );
		ImGui::ColorEdit3( "Ambient",&cbData.ambient.x );
		
		ImGui::Text( "Falloff" );
		ImGui::SliderFloat( "Constant",&cbData.attConst,0.05f,10.0f,"%.2f",0 );
		ImGui::SliderFloat( "Linear",&cbData.attLin,0.0001f,4.0f,"%.4f", ImGuiSliderFlags_Logarithmic);
		ImGui::SliderFloat( "Quadratic",&cbData.attQuad,0.0000001f,10.0f,"%.7f", ImGuiSliderFlags_Logarithmic);

		if( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset(DirectX::XMFLOAT3 position) noexcept
{
	cbData = {
		{ 0.0f,0.0f,0.0f },//pos
		{ 0.0f,0.0f,0.0f },//amb
		{ 1.0f,1.0f,1.0f },//diffuse
		2.0f,//dint
		1.0f,//att0
		0.022f,//att1
		0.0019f,//att2
	};
}

void PointLight::Draw( Graphics& gfx ) const noexcept(!IS_DEBUG)
{
	mesh.UpdatePosition( cbData.pos );
	mesh.Draw( gfx );
}

//void PointLight::Bind( Graphics& gfx,DirectX::FXMMATRIX view ) const noexcept
//{
//	auto dataCopy = cbData;
//	const auto pos = DirectX::XMLoadFloat3( &cbData.pos );
//	DirectX::XMStoreFloat3( &dataCopy.pos,DirectX::XMVector3Transform( pos,view ) );
//	cbuf.Update( gfx,dataCopy );
//	cbuf.Bind( gfx );
//}
