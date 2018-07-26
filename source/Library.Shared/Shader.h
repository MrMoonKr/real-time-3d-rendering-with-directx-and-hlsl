#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "RTTI.h"
#include <d3d11.h>
#include <winrt\Windows.Foundation.h>
#include <gsl/gsl>

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

		winrt::com_ptr<ID3D11ClassLinkage> ClassLinkage() const;
		void SetClassLinkage(winrt::com_ptr<ID3D11ClassLinkage> classLinkage);

		static winrt::com_ptr<ID3D11ClassLinkage> CreateClassLinkage(gsl::not_null<ID3D11Device*> device);

	protected:
		Shader() = default;
	};
}