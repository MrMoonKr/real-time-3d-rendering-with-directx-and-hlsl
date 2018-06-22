#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winrt\Windows.Foundation.h>
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

		winrt::com_ptr<ID3D11PixelShader> Shader() const;

	private:
		friend class PixelShaderReader;
		PixelShader(const winrt::com_ptr<ID3D11PixelShader>& pixelShader);

		winrt::com_ptr<ID3D11PixelShader> mShader;
	};
}
