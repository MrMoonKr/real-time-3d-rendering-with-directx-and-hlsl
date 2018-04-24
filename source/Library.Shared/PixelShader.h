#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <vector>
#include <cstdint>
#include <gsl\gsl>
#include "Shader.h"

namespace Library
{
	class PixelShader final : public Shader
	{
		RTTI_DECLARATIONS(PixelShader, Shader)

	public:
		PixelShader(const PixelShader&) = default;
		PixelShader& operator=(const PixelShader&) = default;
		PixelShader(PixelShader&&) = default;
		PixelShader& operator=(PixelShader&&) = default;
		~PixelShader() = default;

		Microsoft::WRL::ComPtr<ID3D11PixelShader> Shader() const;

	private:
		friend class PixelShaderReader;
		PixelShader(const Microsoft::WRL::ComPtr<ID3D11PixelShader>& pixelShader);

		Microsoft::WRL::ComPtr<ID3D11PixelShader> mShader;
	};
}
