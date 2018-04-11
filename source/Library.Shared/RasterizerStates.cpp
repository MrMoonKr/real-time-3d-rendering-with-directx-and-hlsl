#include "pch.h"
#include "RasterizerStates.h"
#include "GameException.h"

namespace Library
{
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStates::BackCulling;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStates::FrontCulling;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStates::DisabledCulling;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStates::Wireframe;

	void RasterizerStates::Initialize(gsl::not_null<ID3D11Device*> direct3DDevice)
	{
		D3D11_RASTERIZER_DESC rasterizerStateDesc;
		ZeroMemory(&rasterizerStateDesc, sizeof(rasterizerStateDesc));
		rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
		rasterizerStateDesc.DepthClipEnable = true;

		ThrowIfFailed(direct3DDevice->CreateRasterizerState(&rasterizerStateDesc, BackCulling.GetAddressOf()), "ID3D11Device::CreateRasterizerState() failed.");

		ZeroMemory(&rasterizerStateDesc, sizeof(rasterizerStateDesc));
		rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
		rasterizerStateDesc.FrontCounterClockwise = true;
		rasterizerStateDesc.DepthClipEnable = true;

		ThrowIfFailed(direct3DDevice->CreateRasterizerState(&rasterizerStateDesc, FrontCulling.GetAddressOf()), "ID3D11Device::CreateRasterizerState() failed.");

		ZeroMemory(&rasterizerStateDesc, sizeof(rasterizerStateDesc));
		rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
		rasterizerStateDesc.DepthClipEnable = true;

		ThrowIfFailed(direct3DDevice->CreateRasterizerState(&rasterizerStateDesc, DisabledCulling.GetAddressOf()), "ID3D11Device::CreateRasterizerState() failed.");

		ZeroMemory(&rasterizerStateDesc, sizeof(rasterizerStateDesc));
		rasterizerStateDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
		rasterizerStateDesc.DepthClipEnable = true;

		ThrowIfFailed(direct3DDevice->CreateRasterizerState(&rasterizerStateDesc, Wireframe.GetAddressOf()), "ID3D11Device::CreateRasterizerState() failed.");
	}

	void RasterizerStates::Shutdown()
	{
		BackCulling = nullptr;
		FrontCulling = nullptr;
		DisabledCulling = nullptr;
		Wireframe = nullptr;
	}
}