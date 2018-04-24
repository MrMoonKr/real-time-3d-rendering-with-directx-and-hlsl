#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <d3d11.h>
#include <gsl\gsl>

namespace Library
{
	class BlendStates final
	{
	public:
		static Microsoft::WRL::ComPtr<ID3D11BlendState> AlphaBlending;
		static Microsoft::WRL::ComPtr<ID3D11BlendState> MultiplicativeBlending;

		static void Initialize(gsl::not_null<ID3D11Device*> direct3DDevice);
		static void Shutdown();

		BlendStates() = delete;
		BlendStates(const BlendStates&) = delete;
		BlendStates& operator=(const BlendStates&) = delete;
		BlendStates(BlendStates&&) = delete;
		BlendStates& operator=(BlendStates&&) = delete;
		~BlendStates() = default;
	};
}