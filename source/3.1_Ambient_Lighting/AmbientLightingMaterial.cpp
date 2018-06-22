#include "pch.h"
#include "AmbientLightingMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"

using namespace std;
using namespace std::string_literals;
using namespace DirectX;
using namespace winrt;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(AmbientLightingMaterial)

	AmbientLightingMaterial::AmbientLightingMaterial(Game& game, std::shared_ptr<Texture2D> texture) :
		Material(game), mTexture(move(texture))
	{
	}

	com_ptr<ID3D11SamplerState> AmbientLightingMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void AmbientLightingMaterial::SetSamplerState(com_ptr<ID3D11SamplerState> samplerState)
	{
		mSamplerState = move(samplerState);
	}

	shared_ptr<Texture2D> AmbientLightingMaterial::Texture() const
	{
		return mTexture;
	}

	void AmbientLightingMaterial::SetTexture(shared_ptr<Texture2D> texture)
	{
		mTexture = move(texture);
	}

	const XMFLOAT4& AmbientLightingMaterial::AmbientColor() const
	{
		return mPixelCBufferPerFrameData.AmbientColor;
	}

	void AmbientLightingMaterial::SetAmbientColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.AmbientColor = color;
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPixelCBufferPerFrame.get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
	}

	uint32_t AmbientLightingMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTexture);
	}

	void AmbientLightingMaterial::Initialize()
	{
		Material::Initialize();

		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\AmbientLightingDemoVS.cso"s);
		mVertexShader->CreateInputLayout<VertexPositionTexture>(mGame->Direct3DDevice());
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\AmbientLightingDemoPS.cso");

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerObject);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerFrame);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerFrame.put()), "ID3D11Device::CreateBuffer() failed.");

		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPixelCBufferPerFrame.get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
	}

	void AmbientLightingMaterial::UpdateTransforms(FXMMATRIX worldViewProjectionMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}

	void AmbientLightingMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		const auto vsConstantBuffers = mVertexCBufferPerObject.get();
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, &vsConstantBuffers);

		const auto psConstantBuffers = mPixelCBufferPerFrame.get();
		direct3DDeviceContext->PSSetConstantBuffers(0, 1, &psConstantBuffers);

		const auto psShaderResources = mTexture->ShaderResourceView().get();
		direct3DDeviceContext->PSSetShaderResources(0, 1, &psShaderResources);

		const auto psSamplers = mSamplerState.get();
		direct3DDeviceContext->PSSetSamplers(0, 1, &psSamplers);
	}
}