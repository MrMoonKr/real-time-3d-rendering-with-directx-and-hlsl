#include "pch.h"
#include "FilteringModesDemo.h"
#include "Utility.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "KeyboardComponent.h"
#include "Camera.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	RTTI_DEFINITIONS(FilteringModesDemo)

	const map<FilteringModes, string> FilteringModesDemo::FilteringModeNames
	{
		{ FilteringModes::Point, "Point"s },
		{ FilteringModes::TriLinear, "Tri-Linear" },
		{ FilteringModes::Anisotropic, "Anisotropic" }
	};

	FilteringModesDemo::FilteringModesDemo(Game& game, const shared_ptr<Camera>& camera) : 
		DrawableGameComponent(game, camera)
	{
	}

	FilteringModes FilteringModesDemo::ActiveFilteringMode() const
	{
		return mActiveFilteringMode;
	}

	void FilteringModesDemo::Initialize()
	{
		// Load a compiled vertex shader
		vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\FilteringModesDemoVS.cso", compiledVertexShader);		
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, mVertexShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\FilteringModesDemoPS.cso", compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, mPixelShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedPixelShader() failed.");

		// Create an input layout
		const D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ThrowIfFailed(mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), mInputLayout.ReleaseAndGetAddressOf()), "ID3D11Device::CreateInputLayout() failed.");
				
		const float size = 10.0f;
		const float halfSize = size / 2.0f;

		// Create a vertex buffer
		const VertexPositionTexture vertices[] =
		{
			VertexPositionTexture(XMFLOAT4(-halfSize, 1.0f, 0.0, 1.0f), XMFLOAT2(0.0f, 1.0f)),
			VertexPositionTexture(XMFLOAT4(-halfSize, size + 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)),
			VertexPositionTexture(XMFLOAT4(halfSize, size + 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)),
			VertexPositionTexture(XMFLOAT4(halfSize, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f))
		};

		CreateVertexBuffer(vertices, not_null<ID3D11Buffer**>(mVertexBuffer.ReleaseAndGetAddressOf()));

		// Create an index buffer
		const uint16_t sourceIndices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		const span<const uint16_t> indices{ sourceIndices };
		mIndexCount = narrow_cast<uint32_t>(indices.size());
		CreateIndexBuffer(indices, not_null<ID3D11Buffer**>(mIndexBuffer.ReleaseAndGetAddressOf()));

		// Create constant buffers
		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(CBufferPerObject);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mCBufferPerObject.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		// Load a texture
		const wstring textureName = L"Content\\Textures\\EarthComposite.dds";
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.c_str(), nullptr, mColorTexture.ReleaseAndGetAddressOf()), "CreateDDSTextureFromFile() failed.");

		// Create texture samplers
		for (FilteringModes mode = FilteringModes(0); mode < FilteringModes::End; mode = FilteringModes(static_cast<int>(mode) + 1))
		{
			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

			switch (mode)
			{
			case FilteringModes::Point:
				samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
				break;

			case FilteringModes::TriLinear:
				samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				break;

			case FilteringModes::Anisotropic:
				samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
				samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
				break;

			default:
				throw GameException("Unsupported texture addressing mode.");
			}

			ThrowIfFailed(mGame->Direct3DDevice()->CreateSamplerState(&samplerDesc, mTextureSamplersByFilteringMode[mode].ReleaseAndGetAddressOf()), "ID3D11Device::CreateSamplerState() failed.");
		}

		mKeyboard = reinterpret_cast<KeyboardComponent*>(mGame->Services().GetService(KeyboardComponent::TypeIdClass()));

		auto updateConstantBufferFunc = [this]() { mUpdateConstantBuffer = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateConstantBufferFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateConstantBufferFunc);
	}

	void FilteringModesDemo::Update(const GameTime&)
	{
		if (mKeyboard != nullptr)
		{
			if (mKeyboard->WasKeyPressedThisFrame(Keys::Space))
			{
				FilteringModes activeMode = FilteringModes(static_cast<int>(mActiveFilteringMode) + 1);
				if (activeMode >= FilteringModes::End)
				{
					activeMode = FilteringModes(0);
				}

				mActiveFilteringMode = activeMode;
			}
		}
	}

	void FilteringModesDemo::Draw(const GameTime&)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		const uint32_t stride = narrow<uint32_t>(sizeof(VertexPositionTexture));
		const uint32_t offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		if (mUpdateConstantBuffer)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			XMMATRIX wvp = worldMatrix * mCamera->ViewProjectionMatrix();
			wvp = XMMatrixTranspose(wvp);
			XMStoreFloat4x4(&mCBufferPerObjectData.WorldViewProjection, wvp);
			direct3DDeviceContext->UpdateSubresource(mCBufferPerObject.Get(), 0, nullptr, &mCBufferPerObjectData, 0, 0);
			mUpdateConstantBuffer = false;
		}

		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mCBufferPerObject.GetAddressOf());
		direct3DDeviceContext->PSSetShaderResources(0, 1, mColorTexture.GetAddressOf());
		direct3DDeviceContext->PSSetSamplers(0, 1, mTextureSamplersByFilteringMode[mActiveFilteringMode].GetAddressOf());

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
	}

	void FilteringModesDemo::CreateVertexBuffer(const span<const VertexPositionTexture>& vertices, not_null<ID3D11Buffer**> vertexBuffer) const
	{
		D3D11_BUFFER_DESC vertexBufferDesc{ 0 };
		vertexBufferDesc.ByteWidth = narrow<uint32_t>(vertices.size_bytes());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData { 0 };
		vertexSubResourceData.pSysMem = &vertices[0];
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}

	void FilteringModesDemo::CreateIndexBuffer(const span<const uint16_t>& indices, not_null<ID3D11Buffer**> indexBuffer) const
	{
		D3D11_BUFFER_DESC indexBufferDesc{ 0 };
		indexBufferDesc.ByteWidth = narrow<uint32_t>(indices.size_bytes());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{ 0 };
		indexSubResourceData.pSysMem = &indices[0];
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&indexBufferDesc, &indexSubResourceData, indexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}
}