#include "pch.h"
#include "DiffuseLightingMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"

using namespace std;
using namespace std::string_literals;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(DiffuseLightingMaterial)

	DiffuseLightingMaterial::DiffuseLightingMaterial(Game& game, std::shared_ptr<Texture2D> texture) :
		Material(game), mTexture(move(texture))
	{
	}

	ComPtr<ID3D11SamplerState> DiffuseLightingMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void DiffuseLightingMaterial::SetSamplerState(ComPtr<ID3D11SamplerState> samplerState)
	{
		mSamplerState = move(samplerState);
	}

	shared_ptr<Texture2D> DiffuseLightingMaterial::Texture() const
	{
		return mTexture;
	}

	void DiffuseLightingMaterial::SetTexture(shared_ptr<Texture2D> texture)
	{
		mTexture = move(texture);
	}

	const XMFLOAT4& DiffuseLightingMaterial::AmbientColor() const
	{
		return mPixelCBufferPerFrameData.AmbientColor;
	}

	void DiffuseLightingMaterial::SetAmbientColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.AmbientColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT3& DiffuseLightingMaterial::LightDirection() const
	{
		return mPixelCBufferPerFrameData.LightDirection;
	}

	void DiffuseLightingMaterial::SetLightDirection(const XMFLOAT3& direction)
	{
		mPixelCBufferPerFrameData.LightDirection = direction;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT4& DiffuseLightingMaterial::LightColor() const
	{
		return mPixelCBufferPerFrameData.LightColor;
	}

	void DiffuseLightingMaterial::SetLightColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.LightColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	uint32_t DiffuseLightingMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTextureNormal);
	}

	void DiffuseLightingMaterial::Initialize()
	{
		Material::Initialize();

		auto direct3DDevice = mGame->Direct3DDevice();
		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\DiffuseLightingDemoVS.cso"s);
		mVertexShader->CreateInputLayout<VertexPositionTextureNormal>(direct3DDevice);
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\DiffuseLightingDemoPS.cso");

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerObject);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerFrame);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerFrame.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerObject.Get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.Get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
	}

	void DiffuseLightingMaterial::UpdateTransforms(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.World, worldMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.Get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}

	void DiffuseLightingMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		if (mPixelCBufferPerFrameDataDirty)
		{
			direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.Get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
			mPixelCBufferPerFrameDataDirty = false;
		}

		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mVertexCBufferPerObject.GetAddressOf());
		direct3DDeviceContext->PSSetConstantBuffers(0, 1, mPixelCBufferPerFrame.GetAddressOf());
		direct3DDeviceContext->PSSetShaderResources(0, 1, mTexture->ShaderResourceView().GetAddressOf());
		direct3DDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	}
}