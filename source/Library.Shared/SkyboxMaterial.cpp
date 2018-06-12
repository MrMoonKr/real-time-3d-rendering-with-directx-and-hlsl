#include "pch.h"
#include "SkyboxMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexDeclarations.h"
#include "TextureCube.h"
#include "RasterizerStates.h"

using namespace std;
using namespace gsl;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace Library
{
	RTTI_DEFINITIONS(SkyboxMaterial)

	SkyboxMaterial::SkyboxMaterial(Game& game, const shared_ptr<TextureCube>& texture, const ComPtr<ID3D11SamplerState>& samplerState) :
		Material(game),
		mTexture(texture), mSamplerState(samplerState)
	{
	}

	ComPtr<ID3D11SamplerState> SkyboxMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void SkyboxMaterial::SetSamplerState(const ComPtr<ID3D11SamplerState>& samplerState)
	{
		mSamplerState = samplerState;
	}

	shared_ptr<TextureCube> SkyboxMaterial::Texture() const
	{
		return mTexture;
	}

	void SkyboxMaterial::SetTexture(shared_ptr<TextureCube> texture)
	{
		mTexture = move(texture);
	}

	uint32_t SkyboxMaterial::VertexSize() const
	{
		return sizeof(VertexPosition);
	}

	void SkyboxMaterial::Initialize()
	{
		Material::Initialize();

		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\SkyboxVS.cso");
		mVertexShader->CreateInputLayout<VertexPosition>(mGame->Direct3DDevice());
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\SkyboxPS.cso");

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mConstantBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");
	}

	void SkyboxMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
		direct3DDeviceContext->PSSetShaderResources(0, 1, mTexture->ShaderResourceView().GetAddressOf());
		direct3DDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
		direct3DDeviceContext->RSSetState(RasterizerStates::DisabledCulling.Get());
	}

	void SkyboxMaterial::EndDraw()
	{
		mGame->Direct3DDeviceContext()->RSSetState(nullptr);
	}

	void SkyboxMaterial::UpdateTransforms(CXMMATRIX worldViewProjectionMatrix)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}
}