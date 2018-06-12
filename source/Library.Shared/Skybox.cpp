#include "pch.h"
#include "Skybox.h"
#include "Game.h"
#include "GameException.h"
#include "FirstPersonCamera.h"
#include "..\Library.Shared\Model.h"
#include "..\Library.Shared\Mesh.h"
#include "SkyboxMaterial.h"
#include "VertexDeclarations.h"
#include "VectorHelper.h"
#include "TextureCube.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Skybox)

	Skybox::Skybox(Game& game, const shared_ptr<Camera>& camera, const wstring& cubeMapFileName, float scale) :
		DrawableGameComponent(game, camera),
		mCubeMapFileName(cubeMapFileName), mScale(scale)
	{
	}

	void Skybox::Initialize()
	{
		const auto model = mGame->Content().Load<Model>(L"Models\\Sphere.obj.bin");
		Mesh* mesh = model->Meshes().at(0).get();
		VertexPositionTexture::CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.ReleaseAndGetAddressOf()));
		mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), not_null<ID3D11Buffer**>(mIndexBuffer.ReleaseAndGetAddressOf()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		auto textureCube = mGame->Content().Load<TextureCube>(mCubeMapFileName);
		mMaterial = make_shared<SkyboxMaterial>(*mGame, textureCube);
		mMaterial->Initialize();

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		auto firstPersonCamera = mCamera->As<FirstPersonCamera>();
		if (firstPersonCamera != nullptr)
		{
			firstPersonCamera->AddPositionUpdatedCallback([this]() {
				const XMFLOAT3& currentPosition = mCamera->Position();
				XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale, mScale, mScale) * XMMatrixTranslation(currentPosition.x, currentPosition.y, currentPosition.z));
			});
		}
	}
	
	void Skybox::Draw(const GameTime&)
	{
		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial->UpdateTransforms(wvp);
			mUpdateMaterial = false;
		}

		mMaterial->DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.Get()), not_null<ID3D11Buffer*>(mIndexBuffer.Get()), mIndexCount);		
	}
}