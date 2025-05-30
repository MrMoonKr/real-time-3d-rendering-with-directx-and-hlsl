#include "pch.h"
#include "VertexShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

using namespace std;
using namespace DirectX;
using namespace winrt;

namespace Library
{
	RTTI_DEFINITIONS(VertexShaderReader)

	VertexShaderReader::VertexShaderReader(Game& game) :
		ContentTypeReader(game, VertexShader::TypeIdClass())
	{
	}

	shared_ptr<VertexShader> VertexShaderReader::_Read(const wstring& assetName)
	{
		com_ptr<ID3D11VertexShader> vertexShader;
		vector<char> compiledVertexShader;
		//Utility::LoadBinaryFile(assetName, compiledVertexShader);
		if (StringHelper::EndsWith(assetName, L".hlsl"))
		{
			Utility::LoadShaderFile(assetName, "main", "vs_5_0", compiledVertexShader);
		}
		else
		{
			Utility::LoadBinaryFile(assetName, compiledVertexShader);
		}

		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, vertexShader.put()), "ID3D11Device::CreatedVertexShader() failed.");
		
		return shared_ptr<VertexShader>(new VertexShader(move(compiledVertexShader), move(vertexShader)));
	}
}