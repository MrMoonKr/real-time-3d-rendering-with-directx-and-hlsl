#include "pch.h"
#include "PixelShader.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace Library
{
	RTTI_DEFINITIONS(PixelShader)

	PixelShader::PixelShader(const ComPtr<ID3D11PixelShader>& vertexShader) :
		mShader(vertexShader)
	{
	}

	ComPtr<ID3D11PixelShader> PixelShader::Shader() const
	{
		return mShader;
	}
}