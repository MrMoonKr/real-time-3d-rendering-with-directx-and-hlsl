#include "pch.h"
#include "BasicMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(BasicMaterial)

	BasicMaterial::BasicMaterial(Game& game) :
		Material(game)
	{
	}	

	void BasicMaterial::SetSurfaceColor(const DirectX::XMFLOAT4& color)
	{
		SetSurfaceColor(reinterpret_cast<const float*>(&color));
	}

	uint32_t BasicMaterial::VertexSize() const
	{
		return sizeof(VertexPosition);
	}

	void BasicMaterial::Initialize()
	{
		Material::Initialize();

		auto direct3DDevice = mGame->Direct3DDevice();
		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\BasicVS.cso");
		mVertexShader->CreateInputLayout(direct3DDevice, not_null<const D3D11_INPUT_ELEMENT_DESC*>(&VertexPosition::InputElements[0]), VertexPosition::InputElementCount);
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\BasicPS.cso");

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVSConstantBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4);		
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPSConstantBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		SetSurfaceColor(Colors::White.f);
	}

	void BasicMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mVSConstantBuffer.GetAddressOf());
		direct3DDeviceContext->PSSetConstantBuffers(0, 1, mPSConstantBuffer.GetAddressOf());
	}

	void BasicMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVSConstantBuffer.Get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}

	void BasicMaterial::SetSurfaceColor(const float* color)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPSConstantBuffer.Get(), 0, nullptr, color, 0, 0);
	}
}