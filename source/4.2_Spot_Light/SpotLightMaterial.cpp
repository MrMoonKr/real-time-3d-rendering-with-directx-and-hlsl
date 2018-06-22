#include "pch.h"
#include "SpotLightMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace winrt;
using namespace DirectX;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(SpotLightMaterial)

	SpotLightMaterial::SpotLightMaterial(Game& game, shared_ptr<Texture2D> colorMap, shared_ptr<Texture2D> specularMap) :
		Material(game), mColorMap(move(colorMap)), mSpecularMap(move(specularMap))
	{
	}

	com_ptr<ID3D11SamplerState> SpotLightMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void SpotLightMaterial::SetSamplerState(com_ptr<ID3D11SamplerState> samplerState)
	{
		mSamplerState = move(samplerState);
	}

	shared_ptr<Texture2D> SpotLightMaterial::ColorMap() const
	{
		return mColorMap;
	}

	void SpotLightMaterial::SetColorMap(shared_ptr<Texture2D> texture)
	{
		mColorMap = move(texture);
	}

	shared_ptr<Texture2D> SpotLightMaterial::SpecularMap() const
	{
		return mSpecularMap;
	}

	void SpotLightMaterial::SetSpecularMap(shared_ptr<Texture2D> texture)
	{
		mSpecularMap = move(texture);
	}

	const XMFLOAT4& SpotLightMaterial::AmbientColor() const
	{
		return mPixelCBufferPerFrameData.AmbientColor;
	}

	void SpotLightMaterial::SetAmbientColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.AmbientColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT3& SpotLightMaterial::LightPosition() const
	{
		return mPixelCBufferPerFrameData.LightPosition;
	}

	void SpotLightMaterial::SetLightPosition(const XMFLOAT3& position)
	{
		mPixelCBufferPerFrameData.LightPosition = position;
		mPixelCBufferPerFrameDataDirty = true;

		mVertexCBufferPerFrameData.LightPosition = position;
		mVertexCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT3& SpotLightMaterial::LightLookAt() const
	{
		return mPixelCBufferPerFrameData.LightLookAt;
	}

	void SpotLightMaterial::SetLightLookAt(const XMFLOAT3& direction)
	{
		mPixelCBufferPerFrameData.LightLookAt = direction;
		mPixelCBufferPerFrameDataDirty = true;
	}

	float SpotLightMaterial::LightRadius() const
	{
		return mVertexCBufferPerFrameData.LightRadius;
	}

	void SpotLightMaterial::SetLightRadius(float radius)
	{
		mVertexCBufferPerFrameData.LightRadius = radius;
		mVertexCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT4& SpotLightMaterial::LightColor() const
	{
		return mPixelCBufferPerFrameData.LightColor;
	}

	void SpotLightMaterial::SetLightColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.LightColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT3& SpotLightMaterial::SpecularColor() const
	{
		return mPixelCBufferPerObjectData.SpecularColor;
	}

	void SpotLightMaterial::SetSpecularColor(const XMFLOAT3& color)
	{
		mPixelCBufferPerObjectData.SpecularColor = color;
		mPixelCBufferPerObjectDataDirty = true;
	}

	float SpotLightMaterial::SpecularPower() const
	{
		return mPixelCBufferPerObjectData.SpecularPower;
	}

	void SpotLightMaterial::SetSpecularPower(float power)
	{
		mPixelCBufferPerObjectData.SpecularPower = power;
		mPixelCBufferPerObjectDataDirty = true;
	}

	float SpotLightMaterial::SpotLightInnerAngle() const
	{
		return mPixelCBufferPerFrameData.SpotLightInnerAngle;
	}

	void SpotLightMaterial::SetSpotLightInnerAngle(float angle)
	{
		mPixelCBufferPerFrameData.SpotLightInnerAngle = angle;
		mPixelCBufferPerFrameDataDirty = true;
	}

	float SpotLightMaterial::SpotLightOuterAngle() const
	{
		return mPixelCBufferPerFrameData.SpotLightOuterAngle;
	}

	void SpotLightMaterial::SetSpotLightOuterAngle(float angle)
	{
		mPixelCBufferPerFrameData.SpotLightOuterAngle = angle;
		mPixelCBufferPerFrameDataDirty = true;
	}

	uint32_t SpotLightMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTextureNormal);
	}

	void SpotLightMaterial::Initialize()
	{
		Material::Initialize();

		auto direct3DDevice = mGame->Direct3DDevice();
		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\SpotLightDemoVS.cso"s);
		mVertexShader->CreateInputLayout<VertexPositionTextureNormal>(direct3DDevice);
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\SpotLightDemoPS.cso");

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerFrame);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerFrame.put()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerObject);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerFrame);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerFrame.put()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerObject);
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerFrame.get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);
	}

	void SpotLightMaterial::UpdateCameraPosition(const XMFLOAT3& position)
	{
		mPixelCBufferPerFrameData.CameraPosition = position;
		mPixelCBufferPerFrameDataDirty = true;
	}

	void SpotLightMaterial::UpdateTransforms(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.World, worldMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}

	void SpotLightMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		if (mVertexCBufferPerFrameDataDirty)
		{
			direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerFrame.get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
			mVertexCBufferPerFrameDataDirty = false;
		}

		if (mPixelCBufferPerFrameDataDirty)
		{
			direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
			mPixelCBufferPerFrameDataDirty = false;
		}

		if (mPixelCBufferPerObjectDataDirty)
		{
			direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);
			mPixelCBufferPerObjectDataDirty = false;
		}

		ID3D11Buffer* const vsConstantBuffers[]{ mVertexCBufferPerFrame.get(), mVertexCBufferPerObject.get() };
		direct3DDeviceContext->VSSetConstantBuffers(0, narrow_cast<uint32_t>(size(vsConstantBuffers)), vsConstantBuffers);

		ID3D11Buffer* const psConstantBuffers[]{ mPixelCBufferPerFrame.get(), mPixelCBufferPerObject.get() };
		direct3DDeviceContext->PSSetConstantBuffers(0, narrow_cast<uint32_t>(size(psConstantBuffers)), psConstantBuffers);

		ID3D11ShaderResourceView* const psShaderResources[] = { mColorMap->ShaderResourceView().get(), mSpecularMap->ShaderResourceView().get() };
		direct3DDeviceContext->PSSetShaderResources(0, narrow_cast<uint32_t>(size(psShaderResources)), psShaderResources);

		const auto psSamplers = mSamplerState.get();
		direct3DDeviceContext->PSSetSamplers(0, 1, &psSamplers);
	}
}