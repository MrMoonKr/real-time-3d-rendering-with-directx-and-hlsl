#include "pch.h"
#include "TextureCube.h"

using namespace Microsoft::WRL;

namespace Library
{
	RTTI_DEFINITIONS(TextureCube)

	TextureCube::TextureCube(const ComPtr<ID3D11ShaderResourceView>& shaderResourceView) :
		Texture(shaderResourceView)
	{
	}
}