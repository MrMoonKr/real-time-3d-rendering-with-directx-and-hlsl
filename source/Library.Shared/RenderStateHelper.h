#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <d3d11_4.h>
#include <cstdint>
#include <array>
#include <limits>
#include <gsl\gsl>

namespace Library
{
	class Game;

	class RenderStateHelper final
	{
	public:
		RenderStateHelper(Game& game);
		RenderStateHelper(const RenderStateHelper&) = delete;
		RenderStateHelper(RenderStateHelper&&) = default;
		RenderStateHelper& operator=(const RenderStateHelper&) = delete;		
		RenderStateHelper& operator=(RenderStateHelper&&) = default;
		~RenderStateHelper() = default;

		static void ResetAll(gsl::not_null<ID3D11DeviceContext*> deviceContext);
		static void ResetRasterizerState(gsl::not_null<ID3D11DeviceContext*> deviceContext);
		static void ResetBlendState(gsl::not_null<ID3D11DeviceContext*> deviceContext);
		static void ResetDepthStencilState(gsl::not_null<ID3D11DeviceContext*> deviceContext);

		ID3D11RasterizerState* RasterizerState();
		ID3D11BlendState* BlendState();
		ID3D11DepthStencilState* DepthStencilState();

		void SaveRasterizerState();
		void RestoreRasterizerState() const;

		void SaveBlendState();
		void RestoreBlendState() const;

		void SaveDepthStencilState();
		void RestoreDepthStencilState() const;

		void SaveAll();
		void RestoreAll() const;

	private:
		Game& mGame;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
		std::array<float, 4> mBlendFactor;
		std::uint32_t mSampleMask{ std::numeric_limits<std::uint32_t>::max() };
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;
		std::uint32_t mStencilRef{ std::numeric_limits<std::uint32_t>::max() };
	};
}