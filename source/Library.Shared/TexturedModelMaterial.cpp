#include "pch.h"
#include "TexturedModelMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"

using namespace std;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(TexturedModelMaterial)

	TexturedModelMaterial::TexturedModelMaterial(Game& game, shared_ptr<Texture2D> texture) :
		Material(game), mTexture(move(texture))
	{
	}

	com_ptr<ID3D11SamplerState> TexturedModelMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void TexturedModelMaterial::SetSamplerState(com_ptr<ID3D11SamplerState> samplerState)
	{
		mSamplerState = move(samplerState);
	}

	shared_ptr<Texture2D> TexturedModelMaterial::Texture() const
	{
		return mTexture;
	}

	void TexturedModelMaterial::SetTexture(shared_ptr<Texture2D> texture)
	{
		mTexture = move(texture);
	}

	uint32_t TexturedModelMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTexture);
	}

	void TexturedModelMaterial::Initialize()
	{
		Material::Initialize();

		auto direct3DDevice = mGame->Direct3DDevice();
		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\TexturedModelVS.cso");
		mVertexShader->CreateInputLayout<VertexPositionTexture>(direct3DDevice);
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\TexturedModelPS.cso");

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");
	}

	void TexturedModelMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		const auto vsConstantBuffers = mVertexCBufferPerObject.get();
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, &vsConstantBuffers);

		if (mTexture != nullptr)
		{
			const auto psShaderResources = mTexture->ShaderResourceView().get();
			direct3DDeviceContext->PSSetShaderResources(0, 1, &psShaderResources);
		}

		const auto psSamplers = mSamplerState.get();
		direct3DDeviceContext->PSSetSamplers(0, 1, &psSamplers);
	}

	void TexturedModelMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}
}