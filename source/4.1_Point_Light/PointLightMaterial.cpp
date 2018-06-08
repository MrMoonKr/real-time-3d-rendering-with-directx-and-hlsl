#include "pch.h"
#include "PointLightMaterial.h"
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
	RTTI_DEFINITIONS(PointLightMaterial)

	PointLightMaterial::PointLightMaterial(Game& game, shared_ptr<Texture2D> colorMap, shared_ptr<Texture2D> specularMap) :
		Material(game), mColorMap(move(colorMap)), mSpecularMap(move(specularMap))
	{
	}

	ComPtr<ID3D11SamplerState> PointLightMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void PointLightMaterial::SetSamplerState(ComPtr<ID3D11SamplerState> samplerState)
	{
		mSamplerState = move(samplerState);
	}

	shared_ptr<Texture2D> PointLightMaterial::ColorMap() const
	{
		return mColorMap;
	}

	void PointLightMaterial::SetColorMap(shared_ptr<Texture2D> texture)
	{
		mColorMap = move(texture);
	}

	shared_ptr<Texture2D> PointLightMaterial::SpecularMap() const
	{
		return mColorMap;
	}

	void PointLightMaterial::SetSpecularMap(shared_ptr<Texture2D> texture)
	{
		mSpecularMap = move(texture);
	}

	const XMFLOAT4& PointLightMaterial::AmbientColor() const
	{
		return mPixelCBufferPerFrameData.AmbientColor;
	}

	void PointLightMaterial::SetAmbientColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.AmbientColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT3& PointLightMaterial::LightPosition() const
	{
		return mPixelCBufferPerFrameData.LightPosition;
	}

	void PointLightMaterial::SetLightPosition(const XMFLOAT3& position)
	{
		mPixelCBufferPerFrameData.LightPosition = position;
		mPixelCBufferPerFrameDataDirty = true;

		mVertexCBufferPerFrameData.LightPosition = position;
		mVertexCBufferPerFrameDataDirty = true;
	}

	const float PointLightMaterial::LightRadius() const
	{
		return mVertexCBufferPerFrameData.LightRadius;
	}

	void PointLightMaterial::SetLightRadius(float radius)
	{
		mVertexCBufferPerFrameData.LightRadius = radius;
		mVertexCBufferPerFrameDataDirty = true;
	}

	const XMFLOAT4& PointLightMaterial::LightColor() const
	{
		return mPixelCBufferPerFrameData.LightColor;
	}

	void PointLightMaterial::SetLightColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.LightColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const DirectX::XMFLOAT3& PointLightMaterial::SpecularColor() const
	{
		return mPixelCBufferPerObjectData.SpecularColor;
	}

	void PointLightMaterial::SetSpecularColor(const DirectX::XMFLOAT3& color)
	{
		mPixelCBufferPerObjectData.SpecularColor = color;
		mPixelCBufferPerObjectDataDirty = true;
	}

	const float PointLightMaterial::SpecularPower() const
	{
		return mPixelCBufferPerObjectData.SpecularPower;
	}

	void PointLightMaterial::SetSpecularPower(float power)
	{
		mPixelCBufferPerObjectData.SpecularPower = power;
		mPixelCBufferPerObjectDataDirty = true;
	}

	uint32_t PointLightMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTextureNormal);
	}

	void PointLightMaterial::Initialize()
	{
		Material::Initialize();

		auto direct3DDevice = mGame->Direct3DDevice();
		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\PointLightDemoVS.cso"s);
		mVertexShader->CreateInputLayout<VertexPositionTextureNormal>(direct3DDevice);
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\PointLightDemoPS.cso");

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

	void PointLightMaterial::UpdateCameraPosition(const DirectX::XMFLOAT3& position)
	{
		mPixelCBufferPerFrameData.CameraPosition = position;
		mPixelCBufferPerFrameDataDirty = true;
	}

	void PointLightMaterial::UpdateTransforms(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.World, worldMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.Get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}

	void PointLightMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		if (mVertexCBufferPerFrameDataDirty)
		{
			direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerFrame.Get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
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
		direct3DDeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(VSConstantBuffers), VSConstantBuffers);
		
		ID3D11Buffer* const PSConstantBuffers[] { mPixelCBufferPerFrame.Get(), mPixelCBufferPerObject.Get() };
		direct3DDeviceContext->PSSetConstantBuffers(0, ARRAYSIZE(PSConstantBuffers), PSConstantBuffers);

		ID3D11ShaderResourceView* const PSShaderResources[] = { mColorMap->ShaderResourceView().Get(), mSpecularMap->ShaderResourceView().Get() };
		direct3DDeviceContext->PSSetShaderResources(0, ARRAYSIZE(PSShaderResources), PSShaderResources);

		direct3DDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	}
}