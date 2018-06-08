#include "pch.h"
#include "PointLightDemo.h"
#include "FirstPersonCamera.h"
#include "VertexDeclarations.h"
#include "Game.h"
#include "GameException.h"
#include "..\Library.Shared\Model.h"
#include "..\Library.Shared\Mesh.h"
#include "ProxyModel.h"
#include "PointLightMaterial.h"
#include "Texture2D.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	PointLightDemo::PointLightDemo(Game & game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	PointLightDemo::~PointLightDemo()
	{
	}

	bool PointLightDemo::AnimationEnabled() const
	{
		return mAnimationEnabled;
	}

	void PointLightDemo::SetAnimationEnabled(bool enabled)
	{
		mAnimationEnabled = enabled;
	}

	void PointLightDemo::ToggleAnimation()
	{
		mAnimationEnabled = !mAnimationEnabled;
	}

	float PointLightDemo::AmbientLightIntensity() const
	{
		return mMaterial->AmbientColor().x;
	}

	void PointLightDemo::SetAmbientLightIntensity(float intensity)
	{
		mMaterial->SetAmbientColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float PointLightDemo::PointLightIntensity() const
	{
		return mMaterial->LightColor().x;
	}

	void PointLightDemo::SetPointLightIntensity(float intensity)
	{
		mMaterial->SetLightColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	const XMFLOAT3& PointLightDemo::LightPosition() const
	{
		return mPointLight.Position();
	}

	const XMVECTOR PointLightDemo::LightPositionVector() const
	{
		return mPointLight.PositionVector();
	}

	void PointLightDemo::SetLightPosition(const XMFLOAT3& position)
	{
		mPointLight.SetPosition(position);
		mProxyModel->SetPosition(position);
		mMaterial->SetLightPosition(position);
	}

	void PointLightDemo::SetLightPosition(FXMVECTOR position)
	{
		mPointLight.SetPosition(position);
		mProxyModel->SetPosition(position);

		XMFLOAT3 materialPosition;
		XMStoreFloat3(&materialPosition, position);
		mMaterial->SetLightPosition(materialPosition);
	}

	float PointLightDemo::LightRadius() const
	{
		return mMaterial->LightRadius();
	}

	void PointLightDemo::SetLightRadius(float radius)
	{
		mMaterial->SetLightRadius(radius);
	}

	float PointLightDemo::SpecularIntensity() const
	{
		return mMaterial->SpecularColor().x;
	}

	void PointLightDemo::SetSpecularIntensity(float intensity)
	{
		mMaterial->SetSpecularColor(XMFLOAT3(intensity, intensity, intensity));
	}

	float PointLightDemo::SpecularPower() const
	{
		return mMaterial->SpecularPower();
	}

	void PointLightDemo::SetSpecularPower(float power)
	{
		mMaterial->SetSpecularPower(power);
	}

	void PointLightDemo::Initialize()
	{
		const auto model = mGame->Content().Load<Model>(L"Models\\Sphere.obj.bin"s);
		Mesh* mesh = model->Meshes().at(0).get();
		CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.ReleaseAndGetAddressOf()));
		mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), not_null<ID3D11Buffer**>(mIndexBuffer.ReleaseAndGetAddressOf()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		auto colorMap = mGame->Content().Load<Texture2D>(L"Textures\\EarthComposite.dds"s);
		auto specularMap = mGame->Content().Load<Texture2D>(L"Textures\\EarthSpecularMap.png"s);
		mMaterial = make_shared<PointLightMaterial>(*mGame, colorMap, specularMap);
		mMaterial->Initialize();
	
		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Models\\PointLightProxy.obj.bin"s, 0.5f);
		mProxyModel->Initialize();		

		SetLightPosition(XMFLOAT3(1.0f, 0.0, 8.0f));

		auto firstPersonCamera = mCamera->As<FirstPersonCamera>();
		if (firstPersonCamera != nullptr)
		{
			firstPersonCamera->AddPositionUpdatedCallback([this]() {
				mMaterial->UpdateCameraPosition(mCamera->Position());
			});
		}

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);
	}

	void PointLightDemo::Update(const GameTime& gameTime)
	{
		if (mAnimationEnabled)
		{
			mModelRotationAngle += gameTime.ElapsedGameTimeSeconds().count() * RotationRate;
			XMStoreFloat4x4(&mWorldMatrix, XMMatrixRotationY(mModelRotationAngle));			
			mUpdateMaterial = true;
		}

		mProxyModel->Update(gameTime);
	}

	void PointLightDemo::Draw(const GameTime& gameTime)
	{
		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial->UpdateTransforms(wvp, XMMatrixTranspose(worldMatrix));
			mUpdateMaterial = false;
		}

		mMaterial->DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.Get()), not_null<ID3D11Buffer*>(mIndexBuffer.Get()), mIndexCount);
		mProxyModel->Draw(gameTime);
	}

	void PointLightDemo::CreateVertexBuffer(not_null<ID3D11Device*> device, const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer) const
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
}