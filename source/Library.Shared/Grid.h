#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <d3d11_4.h>
#include <DirectXMath.h>
#include "DrawableGameComponent.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"

namespace Library
{
	class Grid final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Grid, DrawableGameComponent)

	public:
		Grid(Game& game, const std::shared_ptr<Camera>& camera, std::uint32_t size = DefaultSize, std::uint32_t scale = DefaultScale, const DirectX::XMFLOAT4& color = DefaultColor);

		Grid() = delete;
		Grid(const Grid&) = delete;
		Grid& operator=(const Grid&) = delete;

		const DirectX::XMFLOAT3& Position() const;
		const DirectX::XMFLOAT4 Color() const;
		const std::uint32_t Size() const;
		const std::uint32_t Scale() const;

		void SetPosition(const DirectX::XMFLOAT3& position);
		void SetPosition(float x, float y, float z);
		void SetColor(const DirectX::XMFLOAT4& color);
		void SetSize(std::uint32_t size);
		void SetScale(std::uint32_t scale);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			VertexCBufferPerObject() { }
			VertexCBufferPerObject(const DirectX::XMFLOAT4X4& wvp) : WorldViewProjection(wvp) { }
		};

		void InitializeGrid();

		inline static const std::uint32_t DefaultSize{ 16 };
		inline static const std::uint32_t DefaultScale{ 16 };
		inline static const DirectX::XMFLOAT4 DefaultColor{ 0.961f, 0.871f, 0.702f, 1.0f };

		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexCBufferPerObject;
		VertexCBufferPerObject mVertexCBufferPerObjectData;
	
		DirectX::XMFLOAT3 mPosition{ Vector3Helper::Zero };
		std::uint32_t mSize{ DefaultSize };
		std::uint32_t mScale{ DefaultScale };
		DirectX::XMFLOAT4 mColor{ DefaultColor };
		DirectX::XMFLOAT4X4 mWorldMatrix{ MatrixHelper::Identity };
	};
}