#include "pch.h"
#include "SpotLightDemo.h"
#include "FirstPersonCamera.h"
#include "VertexDeclarations.h"
#include "Game.h"
#include "GameException.h"
#include "..\Library.Shared\Model.h"
#include "..\Library.Shared\Mesh.h"
#include "ProxyModel.h"
#include "SpotLightMaterial.h"
#include "Texture2D.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	SpotLightDemo::SpotLightDemo(Game & game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	SpotLightDemo::~SpotLightDemo()
	{
	}

	float SpotLightDemo::AmbientLightIntensity() const
	{
		return mMaterial->AmbientColor().x;
	}

	void SpotLightDemo::SetAmbientLightIntensity(float intensity)
	{
		mMaterial->SetAmbientColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float SpotLightDemo::SpotLightIntensity() const
	{
		return mMaterial->LightColor().x;
	}

	void SpotLightDemo::SetSpotLightIntensity(float intensity)
	{
		mMaterial->SetLightColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	const XMFLOAT3& SpotLightDemo::LightPosition() const
	{
		return mSpotLight.Position();
	}

	const XMVECTOR SpotLightDemo::LightPositionVector() const
	{
		return mSpotLight.PositionVector();
	}

	void SpotLightDemo::SetLightPosition(const XMFLOAT3& position)
	{
		mSpotLight.SetPosition(position);
		mProxyModel->SetPosition(position);
		mMaterial->SetLightPosition(position);
	}

	void SpotLightDemo::SetLightPosition(FXMVECTOR position)
	{
		mSpotLight.SetPosition(position);
		mProxyModel->SetPosition(position);

		XMFLOAT3 materialPosition;
		XMStoreFloat3(&materialPosition, position);
		mMaterial->SetLightPosition(materialPosition);
	}

	const XMFLOAT3& SpotLightDemo::LightLookAt() const
	{
		return mSpotLight.Direction();
	}

	void SpotLightDemo::RotateSpotLight(const DirectX::XMFLOAT2& amount)
	{
		XMMATRIX lightRotationMatrix = XMMatrixRotationY(amount.x) * XMMatrixRotationAxis(mSpotLight.RightVector(), amount.y);
		mSpotLight.ApplyRotation(lightRotationMatrix);
		mProxyModel->ApplyRotation(lightRotationMatrix);
		mMaterial->SetLightLookAt(mSpotLight.DirectionToLight());
	}

	float SpotLightDemo::LightRadius() const
	{
		return mMaterial->LightRadius();
	}

	void SpotLightDemo::SetLightRadius(float radius)
	{
		mMaterial->SetLightRadius(radius);
	}

	float SpotLightDemo::SpecularIntensity() const
	{
		return mMaterial->SpecularColor().x;
	}

	void SpotLightDemo::SetSpecularIntensity(float intensity)
	{
		mMaterial->SetSpecularColor(XMFLOAT3(intensity, intensity, intensity));
	}

	float SpotLightDemo::SpecularPower() const
	{
		return mMaterial->SpecularPower();
	}

	void SpotLightDemo::SetSpecularPower(float power)
	{
		mMaterial->SetSpecularPower(power);
	}

	float SpotLightDemo::SpotLightInnerAngle() const
	{
		return mMaterial->SpotLightInnerAngle();
	}

	void SpotLightDemo::SetSpotLightInnerAngle(float angle)
	{
		mMaterial->SetSpotLightInnerAngle(angle);
	}

	float SpotLightDemo::SpotLightOuterAngle() const
	{
		return mMaterial->SpotLightOuterAngle();
	}

	void SpotLightDemo::SetSpotLightOuterAngle(float angle)
	{
		mMaterial->SetSpotLightOuterAngle(angle);
	}

	void SpotLightDemo::Initialize()
	{
		// Create a vertex buffer
		VertexPositionTextureNormal vertices[] =
		{
			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), Vector3Helper::Backward),

			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), Vector3Helper::Backward),
		};

		mVertexCount = narrow<uint32_t>(ARRAYSIZE(vertices));
		CreateVertexBuffer(mGame->Direct3DDevice(), vertices, mVertexCount, &mVertexBuffer);

		auto colorMap = mGame->Content().Load<Texture2D>(L"Textures\\Checkerboard.png"s);
		auto specularMap = mGame->Content().Load<Texture2D>(L"Textures\\CheckerboardSpecularMap.png"s);
		mMaterial = make_shared<SpotLightMaterial>(*mGame, colorMap, specularMap);
		mMaterial->Initialize();
	
		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Models\\SpotLightProxy.obj.bin"s, 0.5f);
		mProxyModel->ApplyRotation(XMMatrixRotationX(XM_PIDIV2));
		mProxyModel->Initialize();		

		SetLightPosition(XMFLOAT3(0.0f, 5.0, 2.0f));
		mMaterial->SetLightLookAt(mSpotLight.DirectionToLight());		

		auto firstPersonCamera = mCamera->As<FirstPersonCamera>();
		if (firstPersonCamera != nullptr)
		{
			firstPersonCamera->SetPositionUpdatedCallback([this]() {
				mMaterial->UpdateCameraPosition(mCamera->Position());
			});
		}

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(10.0f, 10.0f, 10.0f));
	}

	void SpotLightDemo::Update(const GameTime& gameTime)
	{
		mProxyModel->Update(gameTime);
	}

	void SpotLightDemo::Draw(const GameTime& gameTime)
	{
		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial->UpdateTransforms(wvp, XMMatrixTranspose(worldMatrix));
			mUpdateMaterial = false;
		}

		mMaterial->Draw(mVertexBuffer.Get(), mVertexCount);
		mProxyModel->Draw(gameTime);
	}

	void SpotLightDemo::CreateVertexBuffer(not_null<ID3D11Device*> device, Library::VertexPositionTextureNormal* vertices, uint32_t vertexCount, not_null<ID3D11Buffer**> vertexBuffer) const
	{
		D3D11_BUFFER_DESC vertexBufferDesc{ 0 };
		vertexBufferDesc.ByteWidth = narrow<uint32_t>(sizeof(VertexPositionTextureNormal) * vertexCount);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{ 0 };
		vertexSubResourceData.pSysMem = &vertices[0];
		ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}
}