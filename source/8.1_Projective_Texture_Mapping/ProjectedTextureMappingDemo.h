#pragma once

#include <gsl\gsl>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <map>
#include "DrawableGameComponent.h"
#include "FullScreenRenderTarget.h"
#include "FullScreenQuad.h"
#include "MatrixHelper.h"

namespace Library
{
	class Texture2D;
}

namespace Rendering
{
	class ProjectedTextureMappingDemo final : public Library::DrawableGameComponent
	{
	public:
		ProjectedTextureMappingDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		ProjectedTextureMappingDemo(const ProjectedTextureMappingDemo&) = delete;
		ProjectedTextureMappingDemo(ProjectedTextureMappingDemo&&) = default;
		ProjectedTextureMappingDemo& operator=(const ProjectedTextureMappingDemo&) = default;		
		ProjectedTextureMappingDemo& operator=(ProjectedTextureMappingDemo&&) = default;
		~ProjectedTextureMappingDemo();

		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		//std::shared_ptr<SpotLightMaterial> mMaterial;
		DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		std::uint32_t mVertexCount{ 0 };
	};
}