#include "pch.h"
#include "AddressingModesDemo.h"
#include "Utility.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "KeyboardComponent.h"
#include "Camera.h"
#include "DirectXHelper.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	RTTI_DEFINITIONS(AddressingModesDemo)

	const map<AddressingModes, string> AddressingModesDemo::AddressingModeNames
	{
		{ AddressingModes::Wrap, "Wrap"s },
		{ AddressingModes::Mirror, "Mirror"s },
		{ AddressingModes::Clamp, "Clamp"s },
		{ AddressingModes::Border, "Border"s }
	};

	AddressingModesDemo::AddressingModesDemo(Game& game, const shared_ptr<Camera>& camera) : 
		DrawableGameComponent(game, camera)
	{
	}

	AddressingModes AddressingModesDemo::ActiveAddressingMode() const
	{
		return mActiveAddressingMode;
	}

	void AddressingModesDemo::Initialize()
	{
		auto direct3DDevice = mGame->Direct3DDevice();

		// Load a compiled vertex shader
		vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\AddressingModesDemoVS.cso", compiledVertexShader);		
		ThrowIfFailed(direct3DDevice->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, mVertexShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\AddressingModesDemoPS.cso", compiledPixelShader);
		ThrowIfFailed(direct3DDevice->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, mPixelShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedPixelShader() failed.");

		// Create an input layout
		const D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ThrowIfFailed(direct3DDevice->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), mInputLayout.ReleaseAndGetAddressOf()), "ID3D11Device::CreateInputLayout() failed.");
				
		const float size = 10.0f;
		const float halfSize = size / 2.0f;

		// Create a vertex buffer
		const VertexPositionTexture vertices[] =
		{
			VertexPositionTexture(XMFLOAT4(-halfSize, 1.0f, 0.0, 1.0f), XMFLOAT2(0.0f, 3.0f)),
			VertexPositionTexture(XMFLOAT4(-halfSize, size + 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)),
			VertexPositionTexture(XMFLOAT4(halfSize, size + 1.0f, 0.0f, 1.0f), XMFLOAT2(3.0f, 0.0f)),
			VertexPositionTexture(XMFLOAT4(halfSize, 1.0f, 0.0f, 1.0f), XMFLOAT2(3.0f, 3.0f))
		};

		VertexPositionTexture::CreateVertexBuffer(direct3DDevice, vertices, not_null<ID3D11Buffer**>(mVertexBuffer.ReleaseAndGetAddressOf()));

		// Create an index buffer
		const uint16_t sourceIndices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		const span<const uint16_t> indices{ sourceIndices };
		mIndexCount = narrow<uint32_t>(indices.size());
		CreateIndexBuffer(direct3DDevice, indices, not_null<ID3D11Buffer**>(mIndexBuffer.ReleaseAndGetAddressOf()));

		// Create constant buffers
		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = narrow<uint32_t>(sizeof(CBufferPerObject));
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mCBufferPerObject.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		// Load a texture
		const wstring textureName = L"Content\\Textures\\Cow.png";
		ThrowIfFailed(DirectX::CreateWICTextureFromFile(direct3DDevice, mGame->Direct3DDeviceContext(), textureName.c_str(), nullptr, mColorTexture.ReleaseAndGetAddressOf()), "CreateWICTextureFromFile() failed.");

		// Create texture samplers
		for (AddressingModes mode = AddressingModes(0); mode < AddressingModes::End; mode = AddressingModes(static_cast<int>(mode) + 1))
		{
			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

			switch (mode)
			{
			case AddressingModes::Wrap:
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				break;

			case AddressingModes::Mirror:
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
				break;

			case AddressingModes::Clamp:
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
				break;

			case AddressingModes::Border:
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
				memcpy(samplerDesc.BorderColor, &Colors::Blue[0], sizeof(XMVECTORF32));
				break;

			default:
				throw GameException("Unsupported texture addressing mode.");
			}

			ThrowIfFailed(direct3DDevice->CreateSamplerState(&samplerDesc, mTextureSamplersByAddressingMode[mode].ReleaseAndGetAddressOf()), "ID3D11Device::CreateSamplerState() failed.");
		}

		mKeyboard = reinterpret_cast<KeyboardComponent*>(mGame->Services().GetService(KeyboardComponent::TypeIdClass()));

		auto updateConstantBufferFunc = [this]() { mUpdateConstantBuffer = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateConstantBufferFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateConstantBufferFunc);
	}

	void AddressingModesDemo::Update(const GameTime&)
	{
		if (mKeyboard != nullptr)
		{
			if (mKeyboard->WasKeyPressedThisFrame(Keys::Space))
			{
				AddressingModes activeMode = AddressingModes(static_cast<int>(mActiveAddressingMode) + 1);
				if (activeMode >= AddressingModes::End)
				{
					activeMode = AddressingModes(0);
				}

				mActiveAddressingMode = activeMode;
			}
		}
	}

	void AddressingModesDemo::Draw(const GameTime&)
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
		direct3DDeviceContext->PSSetSamplers(0, 1, mTextureSamplersByAddressingMode[mActiveAddressingMode].GetAddressOf());

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
	}
}