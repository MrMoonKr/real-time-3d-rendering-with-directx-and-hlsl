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
using namespace winrt;

namespace Library
{
	RTTI_DEFINITIONS(SkyboxMaterial)

	SkyboxMaterial::SkyboxMaterial(Game& game, const shared_ptr<TextureCube>& texture, const com_ptr<ID3D11SamplerState>& samplerState) :
		Material(game),
		mTexture(texture), mSamplerState(samplerState)
	{
	}

	com_ptr<ID3D11SamplerState> SkyboxMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void SkyboxMaterial::SetSamplerState(const com_ptr<ID3D11SamplerState>& samplerState)
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
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mConstantBuffer.put()), "ID3D11Device::CreateBuffer() failed.");
	}

	void SkyboxMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		const auto vsConstantBuffers = mConstantBuffer.get();
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, &vsConstantBuffers);
		
		const auto psShaderResources = mTexture->ShaderResourceView().get();
		direct3DDeviceContext->PSSetShaderResources(0, 1, &psShaderResources);

		const auto psSamplers = mSamplerState.get();
		direct3DDeviceContext->PSSetSamplers(0, 1, &psSamplers);

		direct3DDeviceContext->RSSetState(RasterizerStates::DisabledCulling.get());		
	}

	void SkyboxMaterial::EndDraw()
	{
		mGame->Direct3DDeviceContext()->RSSetState(nullptr);
	}

	void SkyboxMaterial::UpdateTransforms(CXMMATRIX worldViewProjectionMatrix)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mConstantBuffer.get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}
}