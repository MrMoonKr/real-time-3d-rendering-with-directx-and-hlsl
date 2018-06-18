#include "pch.h"
#include "TransparencyMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "BlendStates.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(TransparencyMaterial)

	TransparencyMaterial::TransparencyMaterial(Game& game, shared_ptr<Texture2D> colorMap, shared_ptr<Texture2D> specularMap, shared_ptr<Texture2D> transparencyMap) :
		Material(game),
		mColorMap(move(colorMap)), mSpecularMap(move(specularMap)),
		mTransparencyMap(move(transparencyMap)), mRenderStateHelper(game)
	{
	}

	ComPtr<ID3D11SamplerState> TransparencyMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void TransparencyMaterial::SetSamplerState(ComPtr<ID3D11SamplerState> samplerState)
	{
		mSamplerState = move(samplerState);
	}

	shared_ptr<Texture2D> TransparencyMaterial::ColorMap() const
	{
		return mColorMap;
	}

	void TransparencyMaterial::SetColorMap(shared_ptr<Texture2D> texture)
	{
		mColorMap = move(texture);
	}

	shared_ptr<Texture2D> TransparencyMaterial::SpecularMap() const
	{
		return mSpecularMap;
	}

	void TransparencyMaterial::SetSpecularMap(shared_ptr<Texture2D> texture)
	{
		mSpecularMap = move(texture);
	}

	const XMFLOAT4& TransparencyMaterial::AmbientColor() const
	{
		return mPixelCBufferPerFrameData.AmbientColor;
	}

	void TransparencyMaterial::SetAmbientColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.AmbientColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT3& TransparencyMaterial::LightDirection() const
	{
		return mPixelCBufferPerFrameData.LightDirection;
	}

	void TransparencyMaterial::SetLightDirection(const XMFLOAT3& direction)
	{
		mPixelCBufferPerFrameData.LightDirection = direction;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT4& TransparencyMaterial::LightColor() const
	{
		return mPixelCBufferPerFrameData.LightColor;
	}

	void TransparencyMaterial::SetLightColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.LightColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const DirectX::XMFLOAT3& TransparencyMaterial::SpecularColor() const
	{
		return mPixelCBufferPerObjectData.SpecularColor;
	}

	void TransparencyMaterial::SetSpecularColor(const DirectX::XMFLOAT3& color)
	{
		mPixelCBufferPerObjectData.SpecularColor = color;
		mPixelCBufferPerObjectDataDirty = true;
	}

	const float TransparencyMaterial::SpecularPower() const
	{
		return mPixelCBufferPerObjectData.SpecularPower;
	}

	void TransparencyMaterial::SetSpecularPower(float power)
	{
		mPixelCBufferPerObjectData.SpecularPower = power;
		mPixelCBufferPerObjectDataDirty = true;
	}

	const float TransparencyMaterial::FogStart() const
	{
		return mVertexCBufferPerFrameData.FogStart;
	}

	void TransparencyMaterial::SetFogStart(float fogStart)
	{
		mVertexCBufferPerFrameData.FogStart = fogStart;
		mVertexCBufferPerFrameDataDirty = true;		
	}

	const float TransparencyMaterial::FogRange() const
	{
		return mVertexCBufferPerFrameData.FogRange;
	}

	void TransparencyMaterial::SetFogRange(float fogRange)
	{
		mVertexCBufferPerFrameData.FogRange = fogRange;
		mVertexCBufferPerFrameDataDirty = true;
	}

	uint32_t TransparencyMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTextureNormal);
	}

	void TransparencyMaterial::Initialize()
	{
		Material::Initialize();

		auto direct3DDevice = mGame->Direct3DDevice();
		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\TransparencyDemoVS.cso"s);
		mVertexShader->CreateInputLayout<VertexPositionTextureNormal>(direct3DDevice);
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\TransparencyDemoPS.cso");

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerFrame);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerFrame.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerObject);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerFrame);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerFrame.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerObject);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerObject.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerFrame.Get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerObject.Get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.Get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerObject.Get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);
	}

	void TransparencyMaterial::UpdateCameraPosition(const DirectX::XMFLOAT3& position)
	{
		mVertexCBufferPerFrameData.CameraPosition = position;
		mVertexCBufferPerFrameDataDirty = true;

		mPixelCBufferPerFrameData.CameraPosition = position;
		mPixelCBufferPerFrameDataDirty = true;		
	}

	void TransparencyMaterial::UpdateTransforms(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.World, worldMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.Get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}

	void TransparencyMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		if (mVertexCBufferPerFrameDataDirty)
		{
			mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerFrame.Get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
			mVertexCBufferPerFrameDataDirty = false;
		}

		if (mPixelCBufferPerFrameDataDirty)
		{
			direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.Get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
			mPixelCBufferPerFrameDataDirty = false;
		}

		if (mPixelCBufferPerObjectDataDirty)
		{
			direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerObject.Get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);
			mPixelCBufferPerObjectDataDirty = false;
		}

		ID3D11Buffer* const VSConstantBuffers[]{ mVertexCBufferPerFrame.Get(), mVertexCBufferPerObject.Get() };
		direct3DDeviceContext->VSSetConstantBuffers(0, narrow_cast<uint32_t>(size(VSConstantBuffers)), VSConstantBuffers);
		
		ID3D11Buffer* const PSConstantBuffers[]{ mPixelCBufferPerFrame.Get(), mPixelCBufferPerObject.Get() };
		direct3DDeviceContext->PSSetConstantBuffers(0, narrow_cast<uint32_t>(size(PSConstantBuffers)), PSConstantBuffers);

		ID3D11ShaderResourceView* const PSShaderResources[]{ mColorMap->ShaderResourceView().Get(), mSpecularMap->ShaderResourceView().Get(), mTransparencyMap->ShaderResourceView().Get() };
		direct3DDeviceContext->PSSetShaderResources(0, narrow_cast<uint32_t>(size(PSShaderResources)), PSShaderResources);

		direct3DDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());

		mRenderStateHelper.SaveBlendState();
		direct3DDeviceContext->OMSetBlendState(BlendStates::AlphaBlending.Get(), 0, 0xFFFFFFFF);		
	}

	void TransparencyMaterial::EndDraw()
	{
		mRenderStateHelper.RestoreBlendState();
	}
}