#pragma once

#include "Material.h"
#include "SamplerStates.h"

namespace Library
{
	class TextureCube;

	class SkyboxMaterial final : public Material
	{
		RTTI_DECLARATIONS(SkyboxMaterial, Material)

	public:
		SkyboxMaterial(Game& game, const std::shared_ptr<TextureCube>& texture, const Microsoft::WRL::ComPtr<ID3D11SamplerState>& samplerState = SamplerStates::TrilinearClamp);
		SkyboxMaterial(const SkyboxMaterial&) = default;
		SkyboxMaterial& operator=(const SkyboxMaterial&) = default;
		SkyboxMaterial(SkyboxMaterial&&) = default;
		SkyboxMaterial& operator=(SkyboxMaterial&&) = default;
		~SkyboxMaterial() = default;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(const Microsoft::WRL::ComPtr<ID3D11SamplerState>& samplerState);

		std::shared_ptr<TextureCube> Texture() const;
		void SetTexture(std::shared_ptr<TextureCube> texture);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		void UpdateConstantBuffer(DirectX::CXMMATRIX worldViewProjectionMatrix);

	private:
		virtual void BeginDraw() override;
		virtual void EndDraw() override;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer;
		std::shared_ptr<TextureCube> mTexture;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
	};
}