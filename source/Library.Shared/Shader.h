#pragma once

#include "RTTI.h"
#include <d3d11.h>

namespace Library
{
	class Shader : public RTTI
	{
		RTTI_DECLARATIONS(Shader, RTTI)

	public:		
		Shader(const Shader&) = default;
		Shader& operator=(const Shader&) = default;
		Shader(Shader&&) = default;
		Shader& operator=(Shader&&) = default;
		virtual ~Shader() = default;

	protected:
		Shader() = default;
	};
}