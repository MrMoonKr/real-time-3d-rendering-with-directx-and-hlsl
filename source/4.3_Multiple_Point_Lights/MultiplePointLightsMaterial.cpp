#include "pch.h"
#include "MultiplePointLightsMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"
#include "ColorHelper.h"

using namespace std;
using namespace std::string_literals;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Library;

namespace Rendering
{
	RTTI_DEFINITIONS(MultiplePointLightsMaterial)

	MultiplePointLightsMaterial::MultiplePointLightsMaterial(Game& game, shared_ptr<Texture2D> colorMap, shared_ptr<Texture2D> specularMap) :
		Material(game), mColorMap(move(colorMap)), mSpecularMap(move(specularMap))
	{
	}

	ComPtr<ID3D11SamplerState> MultiplePointLightsMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void MultiplePointLightsMaterial::SetSamplerState(ComPtr<ID3D11SamplerState> samplerState)
	{
		mSamplerState = move(samplerState);
	}

	shared_ptr<Texture2D> MultiplePointLightsMaterial::ColorMap() const
	{
		return mColorMap;
	}

	void MultiplePointLightsMaterial::SetColorMap(shared_ptr<Texture2D> texture)
	{
		mColorMap = move(texture);
	}

	shared_ptr<Texture2D> MultiplePointLightsMaterial::SpecularMap() const
	{
		return mColorMap;
	}

	void MultiplePointLightsMaterial::SetSpecularMap(shared_ptr<Texture2D> texture)
	{
		mSpecularMap = move(texture);
	}

	const XMFLOAT4& MultiplePointLightsMaterial::AmbientColor() const
	{
		return mPixelCBufferPerFrameData.AmbientColor;
	}

	void MultiplePointLightsMaterial::SetAmbientColor(const XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.AmbientColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const array<PointLight, 4>& MultiplePointLightsMaterial::PointLights() const
	{
		return mLights;
	}

	void MultiplePointLightsMaterial::SetPointLight(const PointLight& light, size_t index)
	{
		assert(index < mLights.size());
		mLights[index] = light;

		auto& vsLightToSet = mVertexCBufferPerFrameData.Lights[index];
		vsLightToSet.Position = light.Position();
		vsLightToSet.Radius = light.Radius();
		mVertexCBufferPerFrameDataDirty = true;

		auto& psLightToSet = mPixelCBufferPerFrameData.Lights[index];
		psLightToSet.Position = light.Position();
		psLightToSet.Color = Vector3Helper::ToFloat3(light.Color());
		mPixelCBufferPerFrameDataDirty = true;
	}

	const float MultiplePointLightsMaterial::SpecularPower() const
	{
		return mPixelCBufferPerObjectData.SpecularPower;
	}

	void MultiplePointLightsMaterial::SetSpecularPower(float power)
	{
		mPixelCBufferPerObjectData.SpecularPower = power;
		mPixelCBufferPerObjectDataDirty = true;
	}

	uint32_t MultiplePointLightsMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTextureNormal);
	}

	void MultiplePointLightsMaterial::Initialize()
	{
		Material::Initialize();

		auto direct3DDevice = mGame->Direct3DDevice();
		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\MultiplePointLightsDemoVS.cso"s);
		mVertexShader->CreateInputLayout<VertexPositionTextureNormal>(direct3DDevice);
		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\MultiplePointLightsDemoPS.cso");

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

	void MultiplePointLightsMaterial::UpdateCameraPosition(const DirectX::XMFLOAT3& position)
	{
		mPixelCBufferPerFrameData.CameraPosition = position;
		mPixelCBufferPerFrameDataDirty = true;
	}

	void MultiplePointLightsMaterial::UpdateTransforms(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.World, worldMatrix);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.Get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	}

	void MultiplePointLightsMaterial::BeginDraw()
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