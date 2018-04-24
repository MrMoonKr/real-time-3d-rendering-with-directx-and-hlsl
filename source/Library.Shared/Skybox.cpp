#include "pch.h"
#include "Skybox.h"
#include "Game.h"
#include "GameException.h"
#include "Camera.h"
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
		CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, mVertexBuffer.ReleaseAndGetAddressOf());
		mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), mIndexBuffer.ReleaseAndGetAddressOf());
		mIndexCount = static_cast<uint32_t>(mesh->Indices().size());

		auto textureCube = mGame->Content().Load<TextureCube>(mCubeMapFileName);
		mMaterial = make_shared<SkyboxMaterial>(*mGame, textureCube);
		mMaterial->Initialize();

		using namespace std::placeholders;
		mMaterial->SetUpdateMaterialCallback(bind(&Skybox::UpdateMaterial, this));
	}

	void Skybox::Update(const GameTime&)
	{		
		const XMFLOAT3& currentPosition = mCamera->Position();
		if (Vector3Helper::Equals(mLastPosition, currentPosition) == false)
		{
			XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale, mScale, mScale) * XMMatrixTranslation(currentPosition.x, currentPosition.y, currentPosition.z));
		}

		mLastPosition = currentPosition;
	}

	void Skybox::Draw(const GameTime&)
	{
		mMaterial->DrawIndexed(mVertexBuffer.Get(), mIndexBuffer.Get(), mIndexCount);
	}

	void Skybox::CreateVertexBuffer(not_null<ID3D11Device*> device, const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer) const
	{
		const vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		const vector<XMFLOAT3>& textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates.size() == sourceVertices.size());

		vector<VertexPositionTexture> vertices;
		vertices.reserve(sourceVertices.size());
		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const XMFLOAT3& position = sourceVertices.at(i);
			const XMFLOAT3& uv = textureCoordinates.at(i);
			vertices.push_back(VertexPositionTexture(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y)));
		}

		D3D11_BUFFER_DESC vertexBufferDesc { 0 };
		vertexBufferDesc.ByteWidth = narrow<uint32_t>(sizeof(VertexPositionTexture) * vertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData { 0 };
		vertexSubResourceData.pSysMem = &vertices[0];
		ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}

	void Skybox::UpdateMaterial()
	{
		const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
		mMaterial->UpdateConstantBuffer(wvp);
	}
}