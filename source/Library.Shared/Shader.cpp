#include "pch.h"
#include "Shader.h"
#include "GameException.h"

#include <d3dcompiler.h>
#include <gsl/gsl>
#include <xutility>

using namespace winrt;
using namespace std;
using namespace gsl;
using namespace DirectX;


namespace Library
{
	RTTI_DEFINITIONS(Shader)

	com_ptr<ID3D11ClassLinkage> Shader::CreateClassLinkage(gsl::not_null<ID3D11Device*> device)
	{
		com_ptr<ID3D11ClassLinkage> classLinkage;
		ThrowIfFailed(device->CreateClassLinkage(classLinkage.put()), "ID3D11Device::CreateClassLinkage() failed.");

		return classLinkage;
	}

	com_ptr<ID3DBlob> Shader::CompileShader( const wchar_t* filename, const char* entryPoint, const char* target )
	{
		com_ptr<ID3DBlob> shaderBlob;
		com_ptr<ID3DBlob> errorBlob;

		HRESULT hr = D3DCompileFromFile( filename, nullptr, nullptr, 
			entryPoint, target,
			D3DCOMPILE_ENABLE_STRICTNESS, 0, 
			shaderBlob.put(), errorBlob.put()
		);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA(static_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			return nullptr;
		}

		return shaderBlob;
	}

	com_ptr<ID3D11InputLayout> Shader::CreateInputLayout(gsl::not_null<ID3D11Device*> device, 
		com_ptr<ID3DBlob> shaderBlob,
		const D3D11_INPUT_ELEMENT_DESC* inputs, int inputCount)
	{
		com_ptr<ID3D11InputLayout> inputLayout;
		ThrowIfFailed(device->CreateInputLayout(inputs, inputCount, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), inputLayout.put()), "ID3D11Device::CreateInputLayout() failed.");

		return inputLayout;
	}

	com_ptr<ID3D11VertexShader> Shader::CreateVertexShader(gsl::not_null<ID3D11Device*> device, 
		com_ptr<ID3DBlob> shaderBlob)
	{
		com_ptr<ID3D11VertexShader> vertexShader;
		ThrowIfFailed(device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, vertexShader.put()), "ID3D11Device::CreateVertexShader() failed.");

		return vertexShader;
	}
	com_ptr<ID3D11PixelShader> Shader::CreatePixelShader(gsl::not_null<ID3D11Device*> device,
		com_ptr<ID3DBlob> shaderBlob)
	{
		com_ptr<ID3D11PixelShader> pixelShader;
		ThrowIfFailed(device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pixelShader.put()), "ID3D11Device::CreatePixelShader() failed.");

		return pixelShader;
	}
}

