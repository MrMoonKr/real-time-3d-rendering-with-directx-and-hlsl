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

namespace Library
{
	class Camera;
	class Mesh;
	class DirectionalLight;
	class ProxyModel;
	class VertexShader;
	class PixelShader;
}

namespace Rendering
{
	class BlinnPhongMaterial;

	class BlinnPhongDemo final : public Library::DrawableGameComponent
	{
	public:
		BlinnPhongDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		BlinnPhongDemo(const BlinnPhongDemo&) = delete;
		BlinnPhongDemo(BlinnPhongDemo&&) = default;
		BlinnPhongDemo& operator=(const BlinnPhongDemo&) = default;		
		BlinnPhongDemo& operator=(BlinnPhongDemo&&) = default;
		~BlinnPhongDemo();

		bool AnimationEnabled() const;
		void SetAnimationEnabled(bool enabled);
		void ToggleAnimation();

		float AmbientLightIntensity() const;
		void SetAmbientLightIntensity(float intensity);

		float DirectionalLightIntensity() const;
		void SetDirectionalLightIntensity(float intensity);

		const DirectX::XMFLOAT3& LightDirection() const;
		void RotateDirectionalLight(DirectX::XMFLOAT2 amount);

		float SpecularIntensity() const;
		void SetSpecularIntensity(float intensity);

		float SpecularPower() const;
		void SetSpecularPower(float power);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void UpdateMaterial();
		void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;

		inline static const float RotationRate{ DirectX::XM_PI };

		std::shared_ptr<BlinnPhongMaterial> mMaterial;
		DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mColorTexture;
		std::uint32_t mIndexCount{ 0 };
		bool mAnimationEnabled{ true };
		std::unique_ptr<Library::DirectionalLight> mDirectionalLight;
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		float mModelRotationAngle{ 0.0f };
	};
}