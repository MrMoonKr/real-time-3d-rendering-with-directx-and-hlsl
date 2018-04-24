#include "pch.h"
#include "DiffuseLightingDemo.h"
#include "Utility.h"
#include "Camera.h"
#include "VertexDeclarations.h"
#include "Game.h"
#include "GameException.h"
#include "..\Library.Shared\Model.h"
#include "..\Library.Shared\Mesh.h"
#include "SamplerStates.h"
#include "ProxyModel.h"
#include "DirectionalLight.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	DiffuseLightingDemo::DiffuseLightingDemo(Game & game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	DiffuseLightingDemo::~DiffuseLightingDemo()
	{
	}

	bool DiffuseLightingDemo::AnimationEnabled() const
	{
		return mAnimationEnabled;
	}

	void DiffuseLightingDemo::SetAnimationEnabled(bool enabled)
	{
		mAnimationEnabled = enabled;
	}

	void DiffuseLightingDemo::ToggleAnimation()
	{
		mAnimationEnabled = !mAnimationEnabled;
	}

	float DiffuseLightingDemo::AmbientLightIntensity() const
	{
		return mPixelCBufferPerFrameData.AmbientColor.x;
	}

	void DiffuseLightingDemo::SetAmbientLightIntensity(float ambientIntensity)
	{
		mPixelCBufferPerFrameData.AmbientColor = XMFLOAT4(ambientIntensity, ambientIntensity, ambientIntensity, 1.0f);
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPixelCBufferPerFrame.Get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
	}

	void DiffuseLightingDemo::Initialize()
	{
		// Load a compiled vertex shader
		vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\DiffuseLightingDemoVS.cso", compiledVertexShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, mVertexShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\DiffuseLightingDemoPS.cso", compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, mPixelShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedPixelShader() failed.");

		// Create an input layout
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ThrowIfFailed(mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), mInputLayout.ReleaseAndGetAddressOf()), "ID3D11Device::CreateInputLayout() failed.");

		// Load the model
		Library::Model model("Content\\Models\\Sphere.obj.bin");

		// Create vertex and index buffers for the model
		Mesh* mesh = model.Meshes().at(0).get();
		CreateVertexBuffer(*mesh, mVertexBuffer.ReleaseAndGetAddressOf());
		mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), mIndexBuffer.ReleaseAndGetAddressOf());
		mIndexCount = static_cast<uint32_t>(mesh->Indices().size());

		D3D11_BUFFER_DESC constantBufferDesc { 0 };
		constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerObject);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerFrame);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerFrame.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		// Initialize pixel shader cbuffer with ambient color
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPixelCBufferPerFrame.Get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);

		// Load a texture
		const wstring textureName = L"Content\\Textures\\EarthComposite.dds"s;
		ThrowIfFailed(CreateDDSTextureFromFile(mGame->Direct3DDevice(), textureName.c_str(), nullptr, mColorTexture.ReleaseAndGetAddressOf()), "CreateDDSTextureFromFile() failed.");

		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Models\\DirectionalLightProxy.obj.bin", 0.5f);
		mProxyModel->Initialize();
		mProxyModel->SetPosition(10.0f, 0.0, 0.0f);
		mProxyModel->ApplyRotation(XMMatrixRotationY(XM_PIDIV2));

		mDirectionalLight = make_unique<DirectionalLight>(*mGame);
		mPixelCBufferPerFrameData.LightDirection = mDirectionalLight->DirectionToLight();
	}

	void DiffuseLightingDemo::Update(const GameTime& gameTime)
	{
		static float angle = 0.0f;

		if (mAnimationEnabled)
		{
			angle += gameTime.ElapsedGameTimeSeconds().count() * RotationRate;
			XMStoreFloat4x4(&mWorldMatrix, XMMatrixRotationY(angle));
		}

		mProxyModel->Update(gameTime);
	}

	void DiffuseLightingDemo::Draw(const GameTime& gameTime)
	{
		assert(mCamera != nullptr);

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		uint32_t stride = narrow<uint32_t>(sizeof(VertexPositionTexture));
		uint32_t offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewProjectionMatrix();
		wvp = XMMatrixTranspose(wvp);
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, wvp);

		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerObject.Get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mVertexCBufferPerObject.GetAddressOf());

		direct3DDeviceContext->PSSetConstantBuffers(0, 1, mPixelCBufferPerFrame.GetAddressOf());
		direct3DDeviceContext->PSSetShaderResources(0, 1, mColorTexture.GetAddressOf());
		direct3DDeviceContext->PSSetSamplers(0, 1, SamplerStates::TrilinearWrap.GetAddressOf());

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);

		mProxyModel->Draw(gameTime);
	}

	void DiffuseLightingDemo::CreateVertexBuffer(const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer) const
	{
		const vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		const auto& sourceUVs = mesh.TextureCoordinates().at(0);

		vector<VertexPositionTexture> vertices;
		vertices.reserve(sourceVertices.size());
		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const XMFLOAT3& position = sourceVertices.at(i);
			const XMFLOAT3& uv = sourceUVs.at(i);
			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y));
		}

		D3D11_BUFFER_DESC vertexBufferDesc { 0 };
		vertexBufferDesc.ByteWidth = sizeof(VertexPositionTexture) * static_cast<uint32_t>(vertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData { 0 };
		vertexSubResourceData.pSysMem = &vertices[0];
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}
}