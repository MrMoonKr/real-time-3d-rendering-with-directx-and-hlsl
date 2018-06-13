#include "pch.h"
#include "EnvironmentMappingDemo.h"
#include "FirstPersonCamera.h"
#include "VertexDeclarations.h"
#include "Game.h"
#include "GameException.h"
#include "..\Library.Shared\Model.h"
#include "..\Library.Shared\Mesh.h"
#include "EnvironmentMappingMaterial.h"
#include "Texture2D.h"
#include "TextureCube.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	EnvironmentMappingDemo::EnvironmentMappingDemo(Game & game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	EnvironmentMappingDemo::~EnvironmentMappingDemo()
	{
	}

	float EnvironmentMappingDemo::AmbientLightIntensity() const
	{
		return mMaterial->AmbientColor().x;
	}

	void EnvironmentMappingDemo::SetAmbientLightIntensity(float intensity)
	{
		mMaterial->SetAmbientColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float EnvironmentMappingDemo::EnvironmentIntensity() const
	{
		return mMaterial->EnvironmentColor().x;
	}

	void EnvironmentMappingDemo::SetEnvironmentIntensity(float intensity)
	{
		mMaterial->SetEnvironmentColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float EnvironmentMappingDemo::ReflectionAmount() const
	{
		return mMaterial->ReflectionAmount();
	}

	void EnvironmentMappingDemo::SetReflectionAmount(float amount)
	{
		mMaterial->SetReflectionAmount(amount);
	}

	void EnvironmentMappingDemo::Initialize()
	{
		auto direct3DDevice = mGame->Direct3DDevice();
		const auto model = mGame->Content().Load<Model>(L"Models\\Sphere.obj.bin"s);
		Mesh* mesh = model->Meshes().at(0).get();
		VertexPositionTextureNormal::CreateVertexBuffer(direct3DDevice, *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.ReleaseAndGetAddressOf()));
		mesh->CreateIndexBuffer(*direct3DDevice, not_null<ID3D11Buffer**>(mIndexBuffer.ReleaseAndGetAddressOf()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		auto colorMap = mGame->Content().Load<Texture2D>(L"Textures\\Checkerboard.png"s);
		auto environmentMap = mGame->Content().Load<TextureCube>(L"Textures\\Maskonaive2_1024.dds"s);
		mMaterial = make_shared<EnvironmentMappingMaterial>(*mGame, colorMap, environmentMap);
		mMaterial->Initialize();

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		auto firstPersonCamera = mCamera->As<FirstPersonCamera>();
		if (firstPersonCamera != nullptr)
		{
			firstPersonCamera->AddPositionUpdatedCallback([this]() {
				mMaterial->UpdateCameraPosition(mCamera->Position());
			});
		}
	}

	void EnvironmentMappingDemo::Draw(const GameTime&)
	{
		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial->UpdateTransforms(wvp, XMMatrixTranspose(worldMatrix));
			mUpdateMaterial = false;
		}

		mMaterial->DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.Get()), not_null<ID3D11Buffer*>(mIndexBuffer.Get()), mIndexCount);
	}
}