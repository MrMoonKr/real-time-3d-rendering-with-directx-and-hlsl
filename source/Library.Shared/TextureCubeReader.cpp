#include "pch.h"
#include "TextureCubeReader.h"
#include "Game.h"
#include "GameException.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace Library
{
	RTTI_DEFINITIONS(TextureCubeReader)

	TextureCubeReader::TextureCubeReader(Game& game) :
		ContentTypeReader(game, TextureCube::TypeIdClass())
	{
	}

	shared_ptr<TextureCube> TextureCubeReader::_Read(const wstring& assetName)
	{
		ComPtr<ID3D11ShaderResourceView> shaderResourceView;	
		ThrowIfFailed(CreateDDSTextureFromFile(mGame->Direct3DDevice(), assetName.c_str(), nullptr, shaderResourceView.ReleaseAndGetAddressOf()), "CreateDDSTextureFromFile() failed.");

		return shared_ptr<TextureCube>(new TextureCube(move(shaderResourceView)));
	}
}