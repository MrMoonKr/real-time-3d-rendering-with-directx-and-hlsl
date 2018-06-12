#include "pch.h"
#include "VertexShader.h"
#include "GameException.h"

using namespace std;
using namespace gsl;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace Library
{
	RTTI_DEFINITIONS(VertexShader)

	VertexShader::VertexShader(const vector<char>& compiledShader, const ComPtr<ID3D11VertexShader>& vertexShader) :
		mCompiledShader(compiledShader), mShader(vertexShader)
	{
	}

	VertexShader::VertexShader(vector<char>&& compiledShader, const ComPtr<ID3D11VertexShader>& vertexShader) :
		mCompiledShader(compiledShader), mShader(vertexShader)
	{
	}

	const vector<char>& VertexShader::CompiledShader() const
	{
		return mCompiledShader;
	}

	ComPtr<ID3D11VertexShader> VertexShader::Shader() const
	{
		return mShader;
	}

	ComPtr<ID3D11InputLayout> VertexShader::InputLayout() const
	{
		return mInputLayout;
	}

	void VertexShader::CreateInputLayout(not_null<ID3D11Device*> device, const span<const D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions, bool releaseCompiledShader)
	{
		ThrowIfFailed(device->CreateInputLayout(&inputElementDescriptions[0], narrow<uint32_t>(inputElementDescriptions.size()), &mCompiledShader[0], mCompiledShader.size(), mInputLayout.ReleaseAndGetAddressOf()), "ID3D11Device::CreateInputLayout() failed.");

		if (releaseCompiledShader)
		{
			mCompiledShader.clear();
			mCompiledShader.shrink_to_fit();
		}
	}
}