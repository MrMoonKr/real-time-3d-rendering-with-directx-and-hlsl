#pragma once

#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <gsl/gsl>
#include "RTTI.h"

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

		static winrt::com_ptr<ID3D11ClassLinkage> CreateClassLinkage(gsl::not_null<ID3D11Device*> device);
		
		static winrt::com_ptr<ID3DBlob> CompileShader( const wchar_t* filename, const char* entryPoint="main", const char* target="vs_5_0" );
		
		static winrt::com_ptr<ID3D11InputLayout> CreateInputLayout(gsl::not_null<ID3D11Device*> device, 
			winrt::com_ptr<ID3DBlob> shaderBlob,
			const D3D11_INPUT_ELEMENT_DESC* inputs, int inputCount );
		
		static winrt::com_ptr<ID3D11VertexShader> CreateVertexShader(gsl::not_null<ID3D11Device*> device, 
			winrt::com_ptr<ID3DBlob> shaderBlob );
		
		static winrt::com_ptr<ID3D11PixelShader> CreatePixelShader(gsl::not_null<ID3D11Device*> device,
			winrt::com_ptr<ID3DBlob> shaderBlob );

	protected:
		Shader() = default;
	};
}