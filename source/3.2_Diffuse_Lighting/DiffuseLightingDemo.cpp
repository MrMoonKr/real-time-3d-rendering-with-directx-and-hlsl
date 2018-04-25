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
#include "DiffuseLightingMaterial.h"
#include "Texture2D.h"

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
		return mMaterial->AmbientColor().x;
	}

	void DiffuseLightingDemo::SetAmbientLightIntensity(float intensity)
	{
		mMaterial->SetAmbientColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float DiffuseLightingDemo::DirectionalLightIntensity() const
	{
		return mMaterial->LightColor().x;
	}

	void DiffuseLightingDemo::SetDirectionalLightIntensity(float intensity)
	{
		mMaterial->SetLightColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	const DirectX::XMFLOAT3& DiffuseLightingDemo::LightDirection() const
	{
		return mDirectionalLight->Direction();
	}

	void DiffuseLightingDemo::RotateDirectionalLight(DirectX::XMFLOAT2 amount)
	{
		XMMATRIX lightRotationMatrix = XMMatrixRotationY(amount.x) * XMMatrixRotationAxis(mDirectionalLight->RightVector(), amount.y);
		mDirectionalLight->ApplyRotation(lightRotationMatrix);
		mProxyModel->ApplyRotation(lightRotationMatrix);
		mMaterial->SetLightDirection(mDirectionalLight->DirectionToLight());
	}

	void DiffuseLightingDemo::Initialize()
	{
		const auto model = mGame->Content().Load<Model>(L"Models\\Sphere.obj.bin"s);
		Mesh* mesh = model->Meshes().at(0).get();
		CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, mVertexBuffer.ReleaseAndGetAddressOf());
		mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), mIndexBuffer.ReleaseAndGetAddressOf());
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		auto texture = mGame->Content().Load<Texture2D>(L"Textures\\EarthComposite.dds"s);
		mMaterial = make_shared<DiffuseLightingMaterial>(*mGame, texture);
		mMaterial->Initialize();

		using namespace std::placeholders;
		mMaterial->SetUpdateMaterialCallback(bind(&DiffuseLightingDemo::UpdateMaterial, this));
		
		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Models\\DirectionalLightProxy.obj.bin"s, 0.5f);
		mProxyModel->Initialize();
		mProxyModel->SetPosition(10.0f, 0.0, 0.0f);
		mProxyModel->ApplyRotation(XMMatrixRotationY(XM_PIDIV2));

		mDirectionalLight = make_unique<DirectionalLight>(*mGame);
		mMaterial->SetLightDirection(mDirectionalLight->DirectionToLight());
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
		mMaterial->DrawIndexed(mVertexBuffer.Get(), mIndexBuffer.Get(), mIndexCount);
		mProxyModel->Draw(gameTime);
	}

	void DiffuseLightingDemo::CreateVertexBuffer(not_null<ID3D11Device*> device, const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer) const
	{
		const vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		const auto& sourceUVs = mesh.TextureCoordinates().at(0);
		assert(sourceUVs.size() == sourceVertices.size());
		const auto& sourceNormals = mesh.Normals();
		assert(sourceNormals.size() == sourceVertices.size());

		vector<VertexPositionTextureNormal> vertices;
		vertices.reserve(sourceVertices.size());
		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const XMFLOAT3& position = sourceVertices.at(i);
			const XMFLOAT3& uv = sourceUVs.at(i);
			const XMFLOAT3& normal = sourceNormals.at(i);
			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), normal);
		}

		D3D11_BUFFER_DESC vertexBufferDesc{ 0 };
		vertexBufferDesc.ByteWidth = narrow<uint32_t>(sizeof(VertexPositionTextureNormal) * vertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{ 0 };
		vertexSubResourceData.pSysMem = &vertices[0];
		ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}

	void DiffuseLightingDemo::UpdateMaterial()
	{
		const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
		mMaterial->UpdateVSCBufferPerObject(wvp, XMMatrixTranspose(worldMatrix));
	}
}