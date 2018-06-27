#include "pch.h"
#include "ColorFilteringDemo.h"
#include "Camera.h"
#include "Game.h"
#include "GameException.h"
#include "DiffuseLightingDemo.h"
#include "FullScreenQuadMaterial.h"
#include "PixelShader.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	ColorFilteringDemo::ColorFilteringDemo(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera),
		mRenderTarget(game),
		mFullScreenQuad(game)
	{
	}

	ColorFilteringDemo::~ColorFilteringDemo()
	{
	}

	shared_ptr<DiffuseLightingDemo> ColorFilteringDemo::DiffuseLighting() const
	{
		return mDiffuseLightingDemo;
	}

	ColorFilters ColorFilteringDemo::ActiveColorFilter() const
	{
		return mActiveColorFilter;
	}

	void ColorFilteringDemo::SetActiveColorFilter(ColorFilters colorFilter)
	{
		mActiveColorFilter = colorFilter;
		mFullScreenQuad.Material()->SetPixelShader(mPixelShadersByColorFilter.at(mActiveColorFilter));
	}
	
	void ColorFilteringDemo::Initialize()
	{
		mDiffuseLightingDemo = make_shared<DiffuseLightingDemo>(*mGame, mCamera);
		mDiffuseLightingDemo->Initialize();

		mFullScreenQuad.Initialize();
		mFullScreenQuad.Material()->SetTexture(mRenderTarget.OutputTexture());
		//mFullScreenQuad.Material()->SetUpdateMaterialCallback([&]
		//{
		//	auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		//	ID3D11ShaderResourceView* const psShaderResources[] = { mRenderTarget.OutputTexture() };
		//	direct3DDeviceContext->PSSetShaderResources(0, narrow_cast<uint32_t>(size(psShaderResources)), psShaderResources);
		//});

		auto& content = mGame->Content();
		mPixelShadersByColorFilter[ColorFilters::GrayScale] = content.Load<PixelShader>(L"Shaders\\GrayScaleColorFilterPS.cso");
		mPixelShadersByColorFilter[ColorFilters::Inverse] = content.Load<PixelShader>(L"Shaders\\InverseColorFilterPS.cso");
		mPixelShadersByColorFilter[ColorFilters::Sepia] = content.Load<PixelShader>(L"Shaders\\SepiaColorFilterPS.cso");
		mPixelShadersByColorFilter[ColorFilters::Generic] = content.Load<PixelShader>(L"Shaders\\GenericColorFilterPS.cso");

		SetActiveColorFilter(ColorFilters::GrayScale);
	}

	void ColorFilteringDemo::Update(const GameTime& gameTime)
	{
		mDiffuseLightingDemo->Update(gameTime);
	}

	void ColorFilteringDemo::Draw(const GameTime& gameTime)
	{
		// Render scene to off-screen render target
		mRenderTarget.Begin();

		mGame->Direct3DDeviceContext()->ClearRenderTargetView(mRenderTarget.RenderTargetView(), Colors::CornflowerBlue.f);
		mGame->Direct3DDeviceContext()->ClearDepthStencilView(mRenderTarget.DepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		mGame->Game::Draw(gameTime);
		mDiffuseLightingDemo->Draw(gameTime);

		mRenderTarget.End();
		
		// Render off-screen texture to a full-screen quad with a color filter shader
		mGame->Direct3DDeviceContext()->ClearRenderTargetView(mGame->RenderTargetView(), Colors::CornflowerBlue.f);
		mGame->Direct3DDeviceContext()->ClearDepthStencilView(mGame->DepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		mFullScreenQuad.Draw(gameTime);
		mGame->UnbindPixelShaderResources(0, 1);
	}
}