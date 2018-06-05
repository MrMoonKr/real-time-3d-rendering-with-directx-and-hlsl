#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "DrawableGameComponent.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"

namespace Library
{
	class BasicMaterial;

	class Grid final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Grid, DrawableGameComponent)

	public:
		explicit Grid(Game& game, const std::shared_ptr<Camera>& camera, std::uint32_t size = DefaultSize, std::uint32_t scale = DefaultScale, const DirectX::XMFLOAT4& color = DefaultColor);
		Grid(const Grid&) = delete;
		Grid(Grid&&) = default;
		Grid& operator=(const Grid&) = delete;
		Grid& operator=(Grid&&) = default;
		~Grid() = default;

		const DirectX::XMFLOAT3& Position() const;
		void SetPosition(const DirectX::XMFLOAT3& position);
		void SetPosition(float x, float y, float z);

		void SetColor(const DirectX::XMFLOAT4& color);

		const std::uint32_t Size() const;
		void SetSize(std::uint32_t size);

		const std::uint32_t Scale() const;
		void SetScale(std::uint32_t scale);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		void InitializeGrid();

		inline static const std::uint32_t DefaultSize{ 16 };
		inline static const std::uint32_t DefaultScale{ 16 };
		inline static const DirectX::XMFLOAT4 DefaultColor{ 0.961f, 0.871f, 0.702f, 1.0f };

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		std::shared_ptr<BasicMaterial> mMaterial;	
		DirectX::XMFLOAT3 mPosition{ Vector3Helper::Zero };
		std::uint32_t mSize{ DefaultSize };
		std::uint32_t mScale{ DefaultScale };
		DirectX::XMFLOAT4 mColor{ DefaultColor };
		DirectX::XMFLOAT4X4 mWorldMatrix{ MatrixHelper::Identity };
		bool mUpdateMaterial{ true };
	};
}