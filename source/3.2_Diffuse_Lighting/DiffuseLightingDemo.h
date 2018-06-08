#pragma once

#include <gsl\gsl>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <cstdint>
#include <memory>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include "DirectionalLight.h"

namespace Library
{
	class Camera;
	class Mesh;
	class ProxyModel;
	class VertexShader;
	class PixelShader;
}

namespace Rendering
{
	class DiffuseLightingMaterial;

	class DiffuseLightingDemo final : public Library::DrawableGameComponent
	{
	public:
		DiffuseLightingDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		DiffuseLightingDemo(const DiffuseLightingDemo&) = delete;
		DiffuseLightingDemo(DiffuseLightingDemo&&) = default;
		DiffuseLightingDemo& operator=(const DiffuseLightingDemo&) = default;		
		DiffuseLightingDemo& operator=(DiffuseLightingDemo&&) = default;
		~DiffuseLightingDemo();

		bool AnimationEnabled() const;
		void SetAnimationEnabled(bool enabled);
		void ToggleAnimation();

		float AmbientLightIntensity() const;
		void SetAmbientLightIntensity(float intensity);

		float DirectionalLightIntensity() const;
		void SetDirectionalLightIntensity(float intensity);

		const DirectX::XMFLOAT3& LightDirection() const;
		void RotateDirectionalLight(DirectX::XMFLOAT2 amount);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void CreateVertexBuffer(const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;

		inline static const float RotationRate{ DirectX::XM_PI };

		std::shared_ptr<DiffuseLightingMaterial> mMaterial;
		DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mColorTexture;
		std::uint32_t mIndexCount{ 0 };
		Library::DirectionalLight mDirectionalLight;
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		float mModelRotationAngle{ 0.0f };
		bool mAnimationEnabled{ true };
		bool mUpdateMaterial{ true };
	};
}