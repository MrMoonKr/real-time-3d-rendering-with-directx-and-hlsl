#include "pch.h"
#include "BasicMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(BasicMaterial)

	BasicMaterial::BasicMaterial(Game& game) :
		Material(game)
	{
	}

	uint32_t BasicMaterial::VertexSize() const
	{
		return sizeof(VertexPositionColor);
	}

	void BasicMaterial::Initialize()
	{
		Material::Initialize();

		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\BasicVS.cso");
		mVertexShader->CreateInputLayout(mGame->Direct3DDevice(), const_cast<D3D11_INPUT_ELEMENT_DESC*>(VertexPositionColor::InputElements), VertexPositionColor::InputElementCount);
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\BasicPS.cso");

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mConstantBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");
	}

	void BasicMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
	}

	void BasicMaterial::UpdateConstantBuffer(CXMMATRIX worldViewProjectionMatrix)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, reinterpret_cast<const float*>(worldViewProjectionMatrix.r), 0, 0);
	}
}