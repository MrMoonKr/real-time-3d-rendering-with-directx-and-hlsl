#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <d3d11.h>
#include <gsl\gsl>

namespace Library
{
	class RasterizerStates final
	{
	public:
		static Microsoft::WRL::ComPtr<ID3D11RasterizerState> BackCulling;
		static Microsoft::WRL::ComPtr<ID3D11RasterizerState> FrontCulling;
		static Microsoft::WRL::ComPtr<ID3D11RasterizerState> DisabledCulling;
		static Microsoft::WRL::ComPtr<ID3D11RasterizerState> Wireframe;

		static void Initialize(gsl::not_null<ID3D11Device*> direct3DDevice);
		static void Shutdown();

		RasterizerStates() = delete;
		RasterizerStates(const RasterizerStates&) = delete;
		RasterizerStates& operator=(const RasterizerStates&) = delete;
		RasterizerStates(RasterizerStates&&) = delete;
		RasterizerStates& operator=(RasterizerStates&&) = delete;
		~RasterizerStates() = default;
	};
}
