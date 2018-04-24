#include "pch.h"
#include "Texture.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace Library
{
	RTTI_DEFINITIONS(Texture)

	Texture::Texture(const ComPtr<ID3D11ShaderResourceView>& shaderResourceView) :
		mShaderResourceView(shaderResourceView)
	{
	}
}