#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

namespace Library
{
	struct VertexPosition
	{
		VertexPosition() = default;

		VertexPosition(const DirectX::XMFLOAT4& position) :
			Position(position) { }

		DirectX::XMFLOAT4 Position;
				
		inline static const D3D11_INPUT_ELEMENT_DESC InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		inline static const size_t InputElementCount{ ARRAYSIZE(InputElements) };
	};

	struct VertexPositionColor
	{
		VertexPositionColor() = default;

		VertexPositionColor(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& color) :
			Position(position), Color(color) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT4 Color;

		inline static const D3D11_INPUT_ELEMENT_DESC InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		inline static const size_t InputElementCount{ ARRAYSIZE(InputElements) };
	};

	struct VertexPositionTexture
	{
		VertexPositionTexture() = default;

		VertexPositionTexture(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates) :
			Position(position), TextureCoordinates(textureCoordinates) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;

		inline static const D3D11_INPUT_ELEMENT_DESC InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		inline static const size_t InputElementCount{ ARRAYSIZE(InputElements) };
	};

	struct VertexPositionSize
	{
		VertexPositionSize() = default;

		VertexPositionSize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& size) :
			Position(position), Size(size) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 Size;

		inline static const D3D11_INPUT_ELEMENT_DESC InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		inline static const size_t InputElementCount{ ARRAYSIZE(InputElements) };
	};

	struct VertexPositionNormal
	{
		VertexPositionNormal() = default;

		VertexPositionNormal(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT3& normal) :
			Position(position), Normal(normal) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT3 Normal;

		inline static const D3D11_INPUT_ELEMENT_DESC InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		inline static const size_t InputElementCount{ ARRAYSIZE(InputElements) };
	};

	struct VertexPositionTextureNormal
	{
		VertexPositionTextureNormal() = default;

		VertexPositionTextureNormal(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normal) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;
		DirectX::XMFLOAT3 Normal;

		inline static const D3D11_INPUT_ELEMENT_DESC InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		inline static const size_t InputElementCount{ ARRAYSIZE(InputElements) };
	};

	struct VertexPositionTextureNormalTangent
	{
		VertexPositionTextureNormalTangent() = default;

		VertexPositionTextureNormalTangent(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT3& tangent) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal), Tangent(tangent) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;

		inline static const D3D11_INPUT_ELEMENT_DESC InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		inline static const size_t InputElementCount{ ARRAYSIZE(InputElements) };
	};

	struct VertexSkinnedPositionTextureNormal
	{
		VertexSkinnedPositionTextureNormal() = default;

		VertexSkinnedPositionTextureNormal(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normal, const DirectX::XMUINT4& boneIndices, const DirectX::XMFLOAT4& boneWeights) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal), BoneIndices(boneIndices), BoneWeights(boneWeights) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMUINT4 BoneIndices;
		DirectX::XMFLOAT4 BoneWeights;

		inline static const D3D11_INPUT_ELEMENT_DESC InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		inline static const size_t InputElementCount{ ARRAYSIZE(InputElements) };
	};
}