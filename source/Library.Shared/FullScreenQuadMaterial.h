#pragma once

#include "Material.h"
#include "SamplerStates.h"

namespace Library
{
	class Texture2D;

	class FullScreenQuadMaterial : public Material
	{
		RTTI_DECLARATIONS(FullScreenQuadMaterial, Material)

	public:
		explicit FullScreenQuadMaterial(Game& game, const winrt::com_ptr<ID3D11SamplerState>& samplerState = SamplerStates::TrilinearClamp);
		FullScreenQuadMaterial(const FullScreenQuadMaterial&) = default;
		FullScreenQuadMaterial& operator=(const FullScreenQuadMaterial&) = default;
		FullScreenQuadMaterial(FullScreenQuadMaterial&&) = default;
		FullScreenQuadMaterial& operator=(FullScreenQuadMaterial&&) = default;
		virtual ~FullScreenQuadMaterial() = default;

		winrt::com_ptr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(const winrt::com_ptr<ID3D11SamplerState>& samplerState);

		void SetTexture(gsl::not_null<ID3D11ShaderResourceView*> texture);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

	private:
		virtual void BeginDraw() override;

		ID3D11ShaderResourceView* mTexture;
		winrt::com_ptr<ID3D11SamplerState> mSamplerState;
	};
}