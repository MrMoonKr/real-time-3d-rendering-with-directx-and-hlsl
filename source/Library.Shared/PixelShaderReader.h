#pragma once

#include "ContentTypeReader.h"
#include "PixelShader.h"

namespace Library
{
	class PixelShaderReader : public ContentTypeReader<PixelShader>
	{
		RTTI_DECLARATIONS(PixelShaderReader, AbstractContentTypeReader)

	public:
		PixelShaderReader(Game& game);
		PixelShaderReader(const PixelShaderReader&) = default;
		PixelShaderReader& operator=(const PixelShaderReader&) = default;
		PixelShaderReader(PixelShaderReader&&) = default;
		PixelShaderReader& operator=(PixelShaderReader&&) = default;
		~PixelShaderReader() = default;

	protected:
		virtual std::shared_ptr<PixelShader> _Read(const std::wstring& assetName) override;
	};
}