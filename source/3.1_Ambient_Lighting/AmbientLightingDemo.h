#pragma once

#include <gsl\gsl>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <cstdint>
#include <memory>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"

namespace Library
{
	class Camera;
	class Mesh;
}

namespace Rendering
{
	class AmbientLightingMaterial;

	class AmbientLightingDemo final : public Library::DrawableGameComponent
	{
	public:
		AmbientLightingDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		
		bool AnimationEnabled() const;
		void SetAnimationEnabled(bool enabled);

		float AmbientLightIntensity() const;
		void SetAmbientLightIntensity(float intensity);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void UpdateMaterial();
		void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;

		inline static const float RotationRate{ DirectX::XM_PI };

		std::shared_ptr<AmbientLightingMaterial> mMaterial;
		DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mColorTexture;
		std::uint32_t mIndexCount{ 0 };
		bool mAnimationEnabled{ true };
		float mModelRotationAngle{ 0.0f };
	};
}
