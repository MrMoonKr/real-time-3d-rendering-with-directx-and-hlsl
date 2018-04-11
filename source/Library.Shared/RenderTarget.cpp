#include "pch.h"
#include "RenderTarget.h"

namespace Library
{
    RTTI_DEFINITIONS(RenderTarget)

    std::stack<RenderTarget::RenderTargetData> RenderTarget::sRenderTargetStack;

	void RenderTarget::Begin(gsl::not_null<ID3D11DeviceContext*> deviceContext, std::uint32_t viewCount, gsl::not_null<ID3D11RenderTargetView**> renderTargetViews, gsl::not_null<ID3D11DepthStencilView*> depthStencilView, const D3D11_VIEWPORT& viewport)
	{
		sRenderTargetStack.push(RenderTargetData(viewCount, renderTargetViews, depthStencilView, viewport));
		deviceContext->OMSetRenderTargets(viewCount, renderTargetViews, depthStencilView);
		deviceContext->RSSetViewports(1, &viewport);
	}

	void RenderTarget::End(gsl::not_null<ID3D11DeviceContext*> deviceContext)
	{
		sRenderTargetStack.pop();

		RenderTargetData renderTargetData = sRenderTargetStack.top();
		deviceContext->OMSetRenderTargets(renderTargetData.ViewCount, renderTargetData.RenderTargetViews, renderTargetData.DepthStencilView);
		deviceContext->RSSetViewports(1, &renderTargetData.Viewport);
	}

	void RenderTarget::RebindCurrentRenderTargets(gsl::not_null<ID3D11DeviceContext*> deviceContext)
	{
		RenderTargetData renderTargetData = sRenderTargetStack.top();
		deviceContext->OMSetRenderTargets(renderTargetData.ViewCount, renderTargetData.RenderTargetViews, renderTargetData.DepthStencilView);
		deviceContext->RSSetViewports(1, &renderTargetData.Viewport);
	}
}