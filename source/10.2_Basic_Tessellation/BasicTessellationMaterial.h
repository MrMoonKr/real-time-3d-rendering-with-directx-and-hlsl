#pragma once

#include <DirectXColors.h>
#include "Material.h"
#include "MatrixHelper.h"

namespace Library
{
	class HullShader;
	class DomainShader;
}

namespace Rendering
{
	class BasicTessellationMaterial : public Library::Material
	{
		RTTI_DECLARATIONS(BasicTessellationMaterial, Library::Material)

	public:
		explicit BasicTessellationMaterial(Library::Game& game);
		BasicTessellationMaterial(const BasicTessellationMaterial&) = default;
		BasicTessellationMaterial& operator=(const BasicTessellationMaterial&) = default;
		BasicTessellationMaterial(BasicTessellationMaterial&&) = default;
		BasicTessellationMaterial& operator=(BasicTessellationMaterial&&) = default;
		virtual ~BasicTessellationMaterial() = default;

		bool ShowQuadTopology() const;
		void SetShowQuadTopology(bool showQuadTopology);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;
		virtual void EndDraw() override;

		void UpdateTransforms(DirectX::FXMMATRIX viewProjectionMatrix);
		
	private:
		struct DomainCBufferPerObject final
		{
			DirectX::XMFLOAT4X4 WorldViewProjection{ Library::MatrixHelper::Identity };
		};
		
		struct QuadHullCBufferPerFrame final
		{
			float TessellationEdgeFactors[4]{ 2.0f, 2.0f, 2.0f, 2.0f };
			float TessellationInsideFactors[2]{ 2.0f, 2.0f };
			DirectX::XMFLOAT2 Padding;
		};

		struct TriHullCBufferPerFrame final
		{
			float TessellationEdgeFactors[3]{ 2.0f, 2.0f, 2.0f };
			float TessellationInsideFactor{ 2.0f };
		};

		void UpdateTopology();

		winrt::com_ptr<ID3D11Buffer> mDomainCBufferPerObject;
		winrt::com_ptr<ID3D11Buffer> mQuadHullCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mTriHullCBufferPerFrame;
		DomainCBufferPerObject mDomainCBufferPerObjectData;
		QuadHullCBufferPerFrame mQuadHullCBufferPerFrameData;
		TriHullCBufferPerFrame mTriHullCBufferPerFrameData;
		std::shared_ptr<Library::HullShader> mQuadHullShader;
		std::shared_ptr<Library::HullShader> mTriHullShader;
		std::shared_ptr<Library::DomainShader> mQuadDomainShader;
		std::shared_ptr<Library::DomainShader> mTriDomainShader;

		bool mShowQuadTopology{ false };
	};
}