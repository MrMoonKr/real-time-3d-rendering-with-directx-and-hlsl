#include "pch.h"
#include "Grid.h"
#include "GameException.h"
#include "Game.h"
#include "Camera.h"
#include "Utility.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "VertexDeclarations.h"
#include "BasicMaterial.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Grid)

	Grid::Grid(Game& game, const std::shared_ptr<Camera>& camera, std::uint32_t size, std::uint32_t scale, const XMFLOAT4& color) :
		DrawableGameComponent(game, camera),
		mMaterial(make_shared<BasicMaterial>(*mGame)),
		mPosition(Vector3Helper::Zero), mSize(size), mScale(scale), mColor(color)
	{
	}
	
	const XMFLOAT3& Grid::Position() const
	{
		return mPosition;
	}

	void Grid::SetPosition(const XMFLOAT3& position)
	{
		mPosition = position;

		XMMATRIX translation = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
		XMStoreFloat4x4(&mWorldMatrix, translation);
	}

	void Grid::SetPosition(float x, float y, float z)
	{
		SetPosition(XMFLOAT3(x, y, z));
	}

	const XMFLOAT4 Grid::Color() const
	{
		return mColor;
	}

	void Grid::SetColor(const XMFLOAT4& color)
	{
		mColor = color;
		InitializeGrid();
	}

	const uint32_t Grid::Size() const
	{
		return mSize;
	}

	void Grid::SetSize(uint32_t size)
	{
		mSize = size;
		InitializeGrid();
	}

	const uint32_t Grid::Scale() const
	{
		return mScale;
	}

	void Grid::SetScale(uint32_t scale)
	{
		mScale = scale;
		InitializeGrid();
	}

	void Grid::Initialize()
	{
		mMaterial->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		mMaterial->Initialize();

		using namespace std::placeholders;
		mMaterial->SetUpdateMaterialCallback(bind(&Grid::UpdateMaterial, this));

		InitializeGrid();
	}

	void Grid::Draw(const GameTime&)
	{
		mMaterial->Draw(mVertexBuffer.Get(), (mSize + 1) * 4, 0);
	}

	void Grid::InitializeGrid()
	{
		mVertexBuffer.Reset();

		ID3D11Device* direct3DDevice = GetGame()->Direct3DDevice();
		int length = 4 * (mSize + 1);
		int size = sizeof(VertexPositionColor) * length;
		std::unique_ptr<VertexPositionColor> vertexData(new VertexPositionColor[length]);		
		VertexPositionColor* vertices = vertexData.get();

		float adjustedScale = mScale * 0.1f;
		float maxPosition = mSize * adjustedScale / 2;

        for (unsigned int i = 0, j = 0; i < mSize + 1; i++, j = 4 * i)
        {
            float position = maxPosition - (i * adjustedScale);

            // Vertical line
			vertices[j] = VertexPositionColor(XMFLOAT4(position, 0.0f, maxPosition, 1.0f), mColor);
            vertices[j + 1] = VertexPositionColor(XMFLOAT4(position, 0.0f, -maxPosition, 1.0f), mColor);

            // Horizontal line
            vertices[j + 2] = VertexPositionColor(XMFLOAT4(maxPosition, 0.0f, position, 1.0f), mColor);
            vertices[j + 3] = VertexPositionColor(XMFLOAT4(-maxPosition, 0.0f, position, 1.0f), mColor);
        }

		D3D11_BUFFER_DESC vertexBufferDesc { 0 };
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = size;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData { 0 };
		vertexSubResourceData.pSysMem = vertices;		
		
		ThrowIfFailed(direct3DDevice->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, mVertexBuffer.GetAddressOf()), "ID3D11Device::CreateBuffer() failed");
	}

	void Grid::UpdateMaterial()
	{
		const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
		mMaterial->UpdateConstantBuffer(wvp);
	}
}