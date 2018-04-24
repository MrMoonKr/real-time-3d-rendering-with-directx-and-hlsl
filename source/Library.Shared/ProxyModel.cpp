#include "pch.h"
#include "ProxyModel.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"
#include "Camera.h"
#include "VertexDeclarations.h"
#include "RasterizerStates.h"
#include "..\Library.Shared\Model.h"
#include "..\Library.Shared\Mesh.h"
#include "BasicMaterial.h"
#include <gsl\gsl>

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(ProxyModel)

	ProxyModel::ProxyModel(Game& game, const shared_ptr<Camera>& camera, const std::string& modelFileName, float scale) :
		DrawableGameComponent(game, camera),
		mModelFileName(modelFileName), mScale(scale),
		mMaterial(make_shared<BasicMaterial>(*mGame))
	{
	}

	const XMFLOAT3& ProxyModel::Position() const
	{
		return mPosition;
	}

	const XMFLOAT3& ProxyModel::Direction() const
	{
		return mDirection;
	}

	const XMFLOAT3& ProxyModel::Up() const
	{
		return mUp;
	}

	const XMFLOAT3& ProxyModel::Right() const
	{
		return mRight;
	}

	XMVECTOR ProxyModel::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	XMVECTOR ProxyModel::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	XMVECTOR ProxyModel::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}

	XMVECTOR ProxyModel::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	bool& ProxyModel::DisplayWireframe()
	{
		return mDisplayWireframe;
	}

	void ProxyModel::SetPosition(float x, float y, float z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void ProxyModel::SetPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&mPosition, position);
	}

	void ProxyModel::SetPosition(const XMFLOAT3& position)
	{
		mPosition = position;
	}

	void ProxyModel::ApplyRotation(CXMMATRIX transform)
	{
		XMVECTOR direction = XMLoadFloat3(&mDirection);
		XMVECTOR up = XMLoadFloat3(&mUp);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&mDirection, direction);
		XMStoreFloat3(&mUp, up);
		XMStoreFloat3(&mRight, right);
	}

	void ProxyModel::ApplyRotation(const XMFLOAT4X4& transform)
	{
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}

	void ProxyModel::Initialize()
	{
		const auto model = mGame->Content().Load<Model>(Utility::ToWideString(mModelFileName));
		Mesh* mesh = model->Meshes().at(0).get();
		CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, mVertexBuffer.ReleaseAndGetAddressOf());
		mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), mIndexBuffer.ReleaseAndGetAddressOf());
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		mMaterial->Initialize();

		using namespace std::placeholders;
		mMaterial->SetUpdateMaterialCallback(bind(&ProxyModel::UpdateMaterial, this));	}

	void ProxyModel::Update(const GameTime&)
	{
		XMMATRIX worldMatrix = XMMatrixIdentity();
		MatrixHelper::SetForward(worldMatrix, mDirection);
		MatrixHelper::SetUp(worldMatrix, mUp);
		MatrixHelper::SetRight(worldMatrix, mRight);
		MatrixHelper::SetTranslation(worldMatrix, mPosition);

		XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale, mScale, mScale) * worldMatrix);
	}

	void ProxyModel::Draw(const GameTime&)
	{
		if (mDisplayWireframe)
		{
			mGame->Direct3DDeviceContext()->RSSetState(RasterizerStates::Wireframe.Get());
			mMaterial->DrawIndexed(mVertexBuffer.Get(), mIndexBuffer.Get(), mIndexCount);
			mGame->Direct3DDeviceContext()->RSSetState(nullptr);
		}
		else
		{
			mMaterial->DrawIndexed(mVertexBuffer.Get(), mIndexBuffer.Get(), mIndexCount);
		}
	}

	void ProxyModel::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();

		std::vector<VertexPositionColor> vertices;
		vertices.reserve(sourceVertices.size());
		if (mesh.VertexColors().size() > 0)
		{
			const std::vector<XMFLOAT4>& vertexColors = mesh.VertexColors().at(0);
			assert(vertexColors.size() == sourceVertices.size());

			for (size_t i = 0; i < sourceVertices.size(); i++)
			{
				const XMFLOAT3& position = sourceVertices.at(i);
				const XMFLOAT4& color = vertexColors.at(i);
				vertices.push_back(VertexPositionColor(XMFLOAT4(position.x, position.y, position.z, 1.0f), color));
			}
		}
		else
		{
			XMFLOAT4 color = XMFLOAT4(reinterpret_cast<const float*>(&Colors::White));
			for (size_t i = 0; i < sourceVertices.size(); i++)
			{
				const XMFLOAT3& position = sourceVertices.at(i);
				vertices.push_back(VertexPositionColor(XMFLOAT4(position.x, position.y, position.z, 1.0f), color));
			}
		}

		D3D11_BUFFER_DESC vertexBufferDesc{ 0 };
		vertexBufferDesc.ByteWidth = narrow<uint32_t>(sizeof(VertexPositionTexture) * vertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{ 0 };
		vertexSubResourceData.pSysMem = &vertices[0];
		ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}

	void ProxyModel::UpdateMaterial()
	{
		const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
		mMaterial->UpdateConstantBuffer(wvp);
	}
}