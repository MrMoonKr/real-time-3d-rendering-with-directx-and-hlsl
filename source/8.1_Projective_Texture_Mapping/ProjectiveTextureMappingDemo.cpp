#include "pch.h"
#include "ProjectiveTextureMappingDemo.h"
#include "Game.h"
#include "Texture2D.h"
#include "TexturedModelMaterial.h"
#include "VertexDeclarations.h"
#include "PerspectiveCamera.h"
#include "VectorHelper.h"
#include "Utility.h"
#include "ProxyModel.h"
#include "Frustum.h"
#include "RenderableFrustum.h"
#include "ProjectiveTextureMappingMaterial.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace winrt;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	ProjectiveTextureMappingDemo::ProjectiveTextureMappingDemo(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	ProjectiveTextureMappingDemo::~ProjectiveTextureMappingDemo()
	{
	}

	float ProjectiveTextureMappingDemo::AmbientLightIntensity() const
	{
		return mMaterial->AmbientColor().x;
	}

	void ProjectiveTextureMappingDemo::SetAmbientLightIntensity(float intensity)
	{
		mMaterial->SetAmbientColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float ProjectiveTextureMappingDemo::PointLightIntensity() const
	{
		return mMaterial->LightColor().x;
	}

	void ProjectiveTextureMappingDemo::SetPointLightIntensity(float intensity)
	{
		mMaterial->SetLightColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float ProjectiveTextureMappingDemo::PointLightRadius() const
	{
		return mMaterial->LightRadius();
	}

	void ProjectiveTextureMappingDemo::SetPointLightRadius(float radius)
	{
		mMaterial->SetLightRadius(radius);
	}

	const XMFLOAT3& ProjectiveTextureMappingDemo::ProjectorPosition() const
	{
		return mPointLight.Position();
	}

	const XMVECTOR ProjectiveTextureMappingDemo::ProjectorPositionVector() const
	{
		return mPointLight.PositionVector();
	}

	void ProjectiveTextureMappingDemo::SetProjectorPosition(const XMFLOAT3& position)
	{
		XMVECTOR positionVector = XMLoadFloat3(&position);
		mPointLight.SetPosition(positionVector);
		mProxyModel->SetPosition(positionVector);
		mProjector->SetPosition(positionVector);
		mRenderableProjectorFrustum->SetPosition(positionVector);
	}

	void ProjectiveTextureMappingDemo::SetProjectorPosition(FXMVECTOR position)
	{
		mPointLight.SetPosition(position);
		mProxyModel->SetPosition(position);
		mProjector->SetPosition(position);
		mRenderableProjectorFrustum->SetPosition(position);
	}

	const XMFLOAT3& ProjectiveTextureMappingDemo::ProjectorDirection() const
	{
		return mProjector->Direction();
	}

	void ProjectiveTextureMappingDemo::RotateProjector(const DirectX::XMFLOAT2& amount)
	{
		XMMATRIX rotationMatrix = XMMatrixRotationY(amount.x) * XMMatrixRotationAxis(mProjector->RightVector(), amount.y);
		mProjector->ApplyRotation(rotationMatrix);
		mRenderableProjectorFrustum->ApplyRotation(rotationMatrix);
		mUpdateMaterial = true;
	}

	void ProjectiveTextureMappingDemo::Initialize()
	{
		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Models\\PointLightProxy.obj.bin"s, 0.5f);
		mProxyModel->Initialize();

		mProjector = make_unique<PerspectiveCamera>(*mGame);
		mProjector->SetNearPlaneDistance(0.5f);
		mProjector->SetFarPlaneDistance(100.0f);
		mProjector->Initialize();		

		mRenderableProjectorFrustum = make_unique<RenderableFrustum>(*mGame, mCamera);
		mRenderableProjectorFrustum->Initialize();
		mRenderableProjectorFrustum->InitializeGeometry(Frustum(mProjector->ViewProjectionMatrix()));
		
		SetProjectorPosition(XMFLOAT3(0.0f, 5.0f, 2.0f));

		auto& content = mGame->Content();
		auto colorMap = content.Load<Texture2D>(L"Textures\\Checkerboard.png"s);

		// Initialize the projected scaling matrix from the size of the projected texture
		auto projectedMap = content.Load<Texture2D>(L"Textures\\ProjectedTexture.png"s);
		InitializeProjectedTextureScalingMatrix(projectedMap->Width(), projectedMap->Height());

		//TODO: depthMap
		mMaterial = make_shared<ProjectiveTextureMappingMaterial>(*mGame, colorMap, projectedMap, nullptr);
		mMaterial->Initialize();

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		const VertexPositionTextureNormal sourceVertices[]
		{
			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), Vector3Helper::Backward),

			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), Vector3Helper::Backward),
		};

		const span<const VertexPositionTextureNormal> vertices{ sourceVertices };
		mVertexCount = narrow_cast<uint32_t>(vertices.size());
		VertexPositionTextureNormal::CreateVertexBuffer(mGame->Direct3DDevice(), vertices, not_null<ID3D11Buffer**>(mVertexBuffer.put()));
		
		// Scale the plane
		XMStoreFloat4x4(&mPlaneWorldMatrix, XMMatrixScaling(10.0f, 10.0f, 10.0f));
	}

	void ProjectiveTextureMappingDemo::Update(const GameTime& gameTime)
	{
		mProjector->Update(gameTime);
		mProxyModel->Update(gameTime);		
		mRenderableProjectorFrustum->Update(gameTime);
	}

	void ProjectiveTextureMappingDemo::Draw(const GameTime& gameTime)
	{
		mProxyModel->Draw(gameTime);
		mRenderableProjectorFrustum->Draw(gameTime);

		// Draw plane
		if (mUpdateMaterial)
		{
			const XMMATRIX planeWorldMatrix = XMLoadFloat4x4(&mPlaneWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(planeWorldMatrix * mCamera->ViewProjectionMatrix());
			XMMATRIX projectiveTextureMatrix = planeWorldMatrix * mProjector->ViewProjectionMatrix() * XMLoadFloat4x4(&mProjectedTextureScalingMatrix);

			mMaterial->UpdateTransforms(wvp, XMMatrixTranspose(planeWorldMatrix), projectiveTextureMatrix);
			mUpdateMaterial = false;
		}

		mMaterial->Draw(not_null<ID3D11Buffer*>(mVertexBuffer.get()), mVertexCount);
	}

	void ProjectiveTextureMappingDemo::InitializeProjectedTextureScalingMatrix(uint32_t textureWidth, uint32_t textureHeight)
	{
		float scalingBiasX = 0.5f + (0.5f / textureWidth);
		float scalingBiasY = 0.5f + (0.5f / textureHeight);

		mProjectedTextureScalingMatrix._11 = 0.5f;
		mProjectedTextureScalingMatrix._22 = -0.5f;
		mProjectedTextureScalingMatrix._33 = 1.0f;
		mProjectedTextureScalingMatrix._41 = scalingBiasX;
		mProjectedTextureScalingMatrix._42 = scalingBiasY;
		mProjectedTextureScalingMatrix._44 = 1.0f;
	}
}