#include "pch.h"
#include "FullScreenQuadMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"

using namespace std;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(FullScreenQuadMaterial)

	FullScreenQuadMaterial::FullScreenQuadMaterial(Game& game, const winrt::com_ptr<ID3D11SamplerState>& samplerState) :
		Material(game),
		mSamplerState(samplerState)
	{
	}

	com_ptr<ID3D11SamplerState> FullScreenQuadMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void FullScreenQuadMaterial::SetSamplerState(const com_ptr<ID3D11SamplerState>& samplerState)
	{
		mSamplerState = samplerState;
	}

	void FullScreenQuadMaterial::SetTexture(ID3D11ShaderResourceView* texture)
	{
		mTextures.clear();

		if (texture != nullptr)
		{
			mTextures.push_back(texture);
		}
	}

	void FullScreenQuadMaterial::SetTextures(gsl::span<ID3D11ShaderResourceView*> textures)
	{
		mTextures = move(vector<ID3D11ShaderResourceView*>(textures.begin(), textures.end()));
	}

	uint32_t FullScreenQuadMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTexture);
	}

	void FullScreenQuadMaterial::Initialize()
	{
		Material::Initialize();

		auto direct3DDevice = mGame->Direct3DDevice();
		mVertexShader = mGame->Content().Load<VertexShader>(L"Shaders\\TexturedQuadPassThroughVS.cso");
		mVertexShader->CreateInputLayout<VertexPositionTexture>(direct3DDevice);
	}

	void FullScreenQuadMaterial::BeginDraw()
	{
		Material::BeginDraw();

		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		direct3DDeviceContext->PSSetShaderResources(0, narrow_cast<uint32_t>(mTextures.size()), mTextures.data());

		const auto psSamplers = mSamplerState.get();
		direct3DDeviceContext->PSSetSamplers(0, 1, &psSamplers);
	}
}