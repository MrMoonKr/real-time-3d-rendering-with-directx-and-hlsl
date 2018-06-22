#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <gsl\gsl>

namespace Library
{
	class SamplerStates final
	{
	public:
		inline static winrt::com_ptr<ID3D11SamplerState> TrilinearWrap;
		inline static winrt::com_ptr<ID3D11SamplerState> TrilinearMirror;
		inline static winrt::com_ptr<ID3D11SamplerState> TrilinearClamp;
		inline static winrt::com_ptr<ID3D11SamplerState> TrilinerBorder;
		inline static winrt::com_ptr<ID3D11SamplerState> PointClamp;
		inline static winrt::com_ptr<ID3D11SamplerState> DepthMap;
		inline static winrt::com_ptr<ID3D11SamplerState> ShadowMap;
		inline static winrt::com_ptr<ID3D11SamplerState> PcfShadowMap;

		inline static DirectX::XMVECTORF32 BorderColor{ 0.0f, 0.0f, 0.0f, 1.0f };
		inline static DirectX::XMVECTORF32 ShadowMapBorderColor{ DirectX::Colors::White };

		static void Initialize(gsl::not_null<ID3D11Device*> direct3DDevice);
		static void Shutdown();

		SamplerStates() = delete;
		SamplerStates(const SamplerStates&) = delete;
		SamplerStates& operator=(const SamplerStates&) = delete;
		SamplerStates(SamplerStates&&) = delete;
		SamplerStates& operator=(SamplerStates&&) = delete;
		~SamplerStates() = default;
	};
}