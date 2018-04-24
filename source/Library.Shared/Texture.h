#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <d3d11.h>
#include "RTTI.h"

namespace Library
{
	class Texture : public RTTI
	{
		RTTI_DECLARATIONS(Texture, RTTI)

	public:		
		Texture(const Texture&) = default;
		Texture& operator=(const Texture&) = default;
		Texture(Texture&&) = default;
		Texture& operator=(Texture&&) = default;
		virtual ~Texture() = default;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView() const;

	protected:
		Texture(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView);

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
	};
}

#include "Texture.inl"