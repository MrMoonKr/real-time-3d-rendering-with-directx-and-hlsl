#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winrt\Windows.Foundation.h>
#include <string>
#include <cstdint>
#include <vector>
#include <functional>
#include <d3d11.h>
#include <DirectXMath.h>
#include <gsl\gsl>
#include "RTTI.h"

namespace Library
{
	class Game;
	class VertexShader;
	class PixelShader;

	class Material : public RTTI
	{
		RTTI_DECLARATIONS(Material, RTTI)

	public:
		explicit Material(Game& game, const std::shared_ptr<VertexShader>& vertexShader = nullptr, const std::shared_ptr<PixelShader>& pixelShader = nullptr, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D_PRIMITIVE_TOPOLOGY Topology() const;
		void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);

		std::shared_ptr<VertexShader> GetVertexShader() const;
		void SetVertexShader(const std::shared_ptr<VertexShader>& vertexShader);

		winrt::com_ptr<ID3D11ClassInstance> VertexShaderClassInstance() const;
		void SetVertexShaderClassInstance(winrt::com_ptr<ID3D11ClassInstance> classInstance);

		std::shared_ptr<PixelShader> GetPixelShader() const;		
		void SetPixelShader(const std::shared_ptr<PixelShader>& pixelShader);

		winrt::com_ptr<ID3D11ClassInstance> PixelShaderClassInstance() const;
		void SetPixelShaderClassInstance(winrt::com_ptr<ID3D11ClassInstance> classInstance);

		std::function<void()> DrawCallback() const;
		void SetDrawCallback(std::function<void()> callback);

		std::function<void()> UpdateMaterialCallback() const;
		void SetUpdateMaterialCallback(std::function<void()> callback);

		virtual void Initialize();
		virtual void Draw();
		virtual void Draw(gsl::not_null<ID3D11Buffer*> vertexBuffer, std::uint32_t vertexCount, std::uint32_t startVertexLocation = 0, std::uint32_t offset = 0);
		virtual void DrawIndexed(gsl::not_null<ID3D11Buffer*> vertexBuffer, gsl::not_null<ID3D11Buffer*> indexBuffer, std::uint32_t indexCount, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t vertexOffset = 0, std::uint32_t indexOffset = 0);
		virtual std::uint32_t VertexSize() const = 0;

	protected:
		virtual void BeginDraw();
		virtual void EndDraw();

		gsl::not_null<Game*> mGame;
		D3D_PRIMITIVE_TOPOLOGY mTopology;
		std::shared_ptr<VertexShader> mVertexShader;
		winrt::com_ptr<ID3D11ClassInstance> mVertexShaderClassInstance;
		std::shared_ptr<PixelShader> mPixelShader;
		winrt::com_ptr<ID3D11ClassInstance> mPixelShaderClassInstance;
		std::function<void()> mDrawCallback;
		std::function<void()> mUpdateMaterialCallback;
	};
}