#pragma once

namespace Library
{
	inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::ShaderResourceView() const
	{
		return mShaderResourceView;
	}
}