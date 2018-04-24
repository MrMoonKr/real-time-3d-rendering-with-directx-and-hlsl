#include "pch.h"
#include "PixelShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace Library
{
	RTTI_DEFINITIONS(PixelShaderReader)

	PixelShaderReader::PixelShaderReader(Game& game) :
		ContentTypeReader(game, PixelShader::TypeIdClass())
	{
	}

	shared_ptr<PixelShader> PixelShaderReader::_Read(const wstring& assetName)
	{
		ComPtr<ID3D11PixelShader> pixelShader;
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(assetName, compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, pixelShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedPixelShader() failed.");
		
		return shared_ptr<PixelShader>(new PixelShader(move(pixelShader)));
	}
}