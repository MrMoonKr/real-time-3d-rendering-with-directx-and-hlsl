#pragma once

#include <gsl\gsl>
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"
#include "BasicTessellationMaterial.h"
#include "RenderStateHelper.h"

namespace Rendering
{
	class BasicTessellationDemo final : public Library::DrawableGameComponent
	{
	public:
		BasicTessellationDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		BasicTessellationDemo(const BasicTessellationDemo&) = delete;
		BasicTessellationDemo(BasicTessellationDemo&&) = default;
		BasicTessellationDemo& operator=(const BasicTessellationDemo&) = default;		
		BasicTessellationDemo& operator=(BasicTessellationDemo&&) = default;
		~BasicTessellationDemo();

		bool ShowQuadTopology() const;
		void SetShowQuadTopology(bool showQuadTopology);

		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		BasicTessellationMaterial mMaterial;
		Library::RenderStateHelper mRenderStateHelper;
		winrt::com_ptr<ID3D11Buffer> mTriVertexBuffer;
		winrt::com_ptr<ID3D11Buffer> mQuadVertexBuffer;
		bool mUpdateMaterial{ true };
	};
}