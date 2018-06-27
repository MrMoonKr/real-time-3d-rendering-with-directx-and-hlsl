#pragma once

#include <gsl\gsl>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"
#include "FullScreenRenderTarget.h"
#include "FullScreenQuad.h"

namespace Library
{
	class Camera;
	class PixelShader;
}

namespace Rendering
{
	class DiffuseLightingDemo;

	enum class ColorFilters
	{
		GrayScale = 0,
		Inverse,
		Sepia,
		Generic,
		End
	};

	class ColorFilteringDemo final : public Library::DrawableGameComponent
	{
	public:
		ColorFilteringDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		ColorFilteringDemo(const ColorFilteringDemo&) = delete;
		ColorFilteringDemo(ColorFilteringDemo&&) = default;
		ColorFilteringDemo& operator=(const ColorFilteringDemo&) = default;		
		ColorFilteringDemo& operator=(ColorFilteringDemo&&) = default;
		~ColorFilteringDemo();

		std::shared_ptr<DiffuseLightingDemo> DiffuseLighting() const;

		ColorFilters ActiveColorFilter() const;
		void SetActiveColorFilter(ColorFilters colorFilter);
		
		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		std::shared_ptr<DiffuseLightingDemo> mDiffuseLightingDemo;		
		Library::FullScreenRenderTarget mRenderTarget;
		Library::FullScreenQuad mFullScreenQuad;
		std::map<ColorFilters, std::shared_ptr<Library::PixelShader>> mPixelShadersByColorFilter;
		ColorFilters mActiveColorFilter;	
	};
}