#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <wrl.h>
#include <cstdint>
#include <string>
#include <sstream>
#include <memory>
#include <functional>

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h>

#include "GameClock.h"
#include "GameTime.h"
#include "ServiceContainer.h"
#include "RenderTarget.h"
#include "ContentManager.h"

namespace Library
{
	class GameComponent;

	class IDeviceNotify
	{
	public:
		virtual ~IDeviceNotify() { };

		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;

	protected:
		IDeviceNotify() { };
	};

    class Game : public RenderTarget
    {
		RTTI_DECLARATIONS(Game, RenderTarget)

    public:
        Game(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;
		virtual ~Game() = default;

		ID3D11Device5* Direct3DDevice() const;
		ID3D11DeviceContext4* Direct3DDeviceContext() const;
		IDXGISwapChain1* SwapChain() const;
		ID3D11RenderTargetView* RenderTargetView() const;
		ID3D11DepthStencilView* DepthStencilView() const;
		SIZE RenderTargetSize() const;
		float AspectRatio() const;
		bool IsFullScreen() const;
		const D3D11_TEXTURE2D_DESC& BackBufferDesc() const;
		const D3D11_VIEWPORT& Viewport() const;
		std::uint32_t MultiSamplingCount() const;
		std::uint32_t MultiSamplingQualityLevels() const;

		const std::vector<std::shared_ptr<GameComponent>>& Components() const;
		const ServiceContainer& Services() const;			

        virtual void Initialize();
		virtual void Run();
		virtual void Shutdown();        

		void UpdateRenderTargetSize();
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify);
		virtual void UnbindPixelShaderResources(std::uint32_t startSlot, std::uint32_t count);
		std::function<void*()> GetWindowCallback() const;

		ContentManager& Content();

    protected:
		virtual void Update(const GameTime& gameTime);
		virtual void Draw(const GameTime& gameTime);
		virtual void HandleDeviceLost();

		virtual void Begin() override;
		virtual void End() override;

		virtual void CreateDeviceIndependentResources();
		virtual void CreateDeviceResources();
		virtual void CreateWindowSizeDependentResources();

		inline static const D3D_FEATURE_LEVEL DefaultFeatureLevel{ D3D_FEATURE_LEVEL_9_1 };
		inline static const std::uint32_t DefaultFrameRate{ 60 };
		inline static const std::uint32_t DefaultMultiSamplingCount{ 4 };
		inline static const std::uint32_t DefaultBufferCount{ 2 };

		Microsoft::WRL::ComPtr<ID3D11Device5> mDirect3DDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext4> mDirect3DDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> mSwapChain;
		D3D_FEATURE_LEVEL mFeatureLevel = DefaultFeatureLevel;

		D3D11_TEXTURE2D_DESC mBackBufferDesc;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		D3D11_VIEWPORT mViewport;

		std::uint32_t mFrameRate{ DefaultFrameRate };
		bool mIsFullScreen{ false };
		std::uint32_t mMultiSamplingCount{ DefaultMultiSamplingCount };
		std::uint32_t mMultiSamplingQualityLevels{ 0 };

		std::function<void*()> mGetWindow;
		std::function<void(SIZE&)> mGetRenderTargetSize;
		SIZE mRenderTargetSize;
		IDeviceNotify* mDeviceNotify;

        GameClock mGameClock;
        GameTime mGameTime;
		std::vector<std::shared_ptr<GameComponent>> mComponents;
		ServiceContainer mServices;
		ContentManager mContentManager;
    };
}