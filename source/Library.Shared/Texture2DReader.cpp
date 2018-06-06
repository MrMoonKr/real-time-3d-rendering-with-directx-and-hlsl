#include "pch.h"
#include "Texture2DReader.h"
#include "Game.h"
#include "GameException.h"
#include "StringHelper.h"
#include "TextureHelper.h"

using namespace std;
using namespace gsl;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace Library
{
	RTTI_DEFINITIONS(Texture2DReader)

	Texture2DReader::Texture2DReader(Game& game) :
		ContentTypeReader(game, Texture2D::TypeIdClass())
	{
	}

	shared_ptr<Texture2D> Texture2DReader::_Read(const wstring& assetName)
	{
		ComPtr<ID3D11Resource> resource;
		ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		if (StringHelper::EndsWith(assetName, L".dds"))
		{
			ThrowIfFailed(CreateDDSTextureFromFile(mGame->Direct3DDevice(), assetName.c_str(), resource.ReleaseAndGetAddressOf(), shaderResourceView.ReleaseAndGetAddressOf()), "CreateDDSTextureFromFile() failed.");
		}
		else
		{
			ThrowIfFailed(CreateWICTextureFromFile(mGame->Direct3DDevice(), assetName.c_str(), resource.ReleaseAndGetAddressOf(), shaderResourceView.ReleaseAndGetAddressOf()), "CreateWICTextureFromFile() failed.");
		}

		ComPtr<ID3D11Texture2D> texture;
		ThrowIfFailed(resource.As(&texture), "Invalid ID3D11Resource returned from CreateTextureFromFile. Should be a ID3D11Texture2D.");
		Point textureSize = TextureHelper::GetTextureSize(not_null<ID3D11Texture2D*>(texture.Get()));

		return make_shared<Texture2D>(move(shaderResourceView), textureSize.X, textureSize.Y);
	}
}