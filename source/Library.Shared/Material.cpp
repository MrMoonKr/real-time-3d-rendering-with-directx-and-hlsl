#include "pch.h"
#include "Material.h"
#include "Game.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Material)

	Material::Material(Game& game, const shared_ptr<VertexShader>& vertexShader, const shared_ptr<PixelShader>& pixelShader, D3D_PRIMITIVE_TOPOLOGY topology) :
		mGame(&game), mVertexShader(vertexShader), mPixelShader(pixelShader), mTopology(topology)
	{
	}

	void Material::Initialize()
	{
	}

	void Material::Draw()
	{
		BeginDraw();

		if (mDrawCallback != nullptr)
		{
			mDrawCallback();
		}

		EndDraw();
	}

	void Material::Draw(not_null<ID3D11Buffer*> vertexBuffer, uint32_t vertexCount, uint32_t startVertexLocation, uint32_t offset)
	{
		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		BeginDraw();

		const uint32_t stride = VertexSize();
		ID3D11Buffer* const vertexBuffers[] { vertexBuffer };
		direct3DDeviceContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, &stride, &offset);

		direct3DDeviceContext->Draw(vertexCount, startVertexLocation);

		EndDraw();
	}

	void Material::DrawIndexed(not_null<ID3D11Buffer*> vertexBuffer, not_null<ID3D11Buffer*> indexBuffer, uint32_t indexCount, DXGI_FORMAT format, uint32_t startIndexLocation, uint32_t baseVertexLocation, uint32_t vertexOffset, uint32_t indexOffset)
	{
		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		BeginDraw();

		const uint32_t stride = VertexSize();
		const uint32_t offset = 0;
		ID3D11Buffer* const vertexBuffers[] { vertexBuffer };
		direct3DDeviceContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, &stride, &vertexOffset);
		direct3DDeviceContext->IASetIndexBuffer(indexBuffer, format, indexOffset);

		direct3DDeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);

		EndDraw();
	}

	D3D_PRIMITIVE_TOPOLOGY Material::Topology() const
	{
		return mTopology;
	}

	void Material::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
	{
		mTopology = topology;
	}

	shared_ptr<VertexShader> Material::GetVertexShader() const
	{
		return mVertexShader;
	}

	void Material::SetVertexShader(const shared_ptr<VertexShader>& vertexShader)
	{
		mVertexShader = vertexShader;
	}

	shared_ptr<PixelShader> Material::GetPixelShader() const
	{
		return mPixelShader;
	}

	void Material::SetPixelShader(const shared_ptr<PixelShader>& pixelShader)
	{
		mPixelShader = pixelShader;
	}

	function<void()> Material::DrawCallback() const
	{
		return mDrawCallback;
	}

	void Material::SetDrawCallback(function<void()> callback)
	{
		mDrawCallback = callback;
	}

	function<void()> Material::UpdateMaterialCallback() const
	{
		return mUpdateMaterialCallback;
	}

	void Material::SetUpdateMaterialCallback(function<void()> callback)
	{
		mUpdateMaterialCallback = callback;
	}

	void Material::BeginDraw()
	{
		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(mTopology);
		direct3DDeviceContext->IASetInputLayout(mVertexShader->InputLayout().Get());
		direct3DDeviceContext->VSSetShader(mVertexShader->Shader().Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader->Shader().Get(), nullptr, 0);

		if (mUpdateMaterialCallback != nullptr)
		{
			mUpdateMaterialCallback();
		}
	}

	void Material::EndDraw()
	{
	}
}