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
#include "PointLight.h"

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
	class PointLightMaterial;

	class PointLightDemo final : public Library::DrawableGameComponent
	{
	public:
		PointLightDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		PointLightDemo(const PointLightDemo&) = delete;
		PointLightDemo(PointLightDemo&&) = default;
		PointLightDemo& operator=(const PointLightDemo&) = default;		
		PointLightDemo& operator=(PointLightDemo&&) = default;
		~PointLightDemo();

		bool AnimationEnabled() const;
		void SetAnimationEnabled(bool enabled);
		void ToggleAnimation();

		float AmbientLightIntensity() const;
		void SetAmbientLightIntensity(float intensity);

		float PointLightIntensity() const;
		void SetPointLightIntensity(float intensity);

		const DirectX::XMFLOAT3& LightPosition() const;
		const DirectX::XMVECTOR LightPositionVector() const;
		void SetLightPosition(const DirectX::XMFLOAT3& position);
		void SetLightPosition(DirectX::FXMVECTOR position);

		float LightRadius() const;
		void SetLightRadius(float radius);

		float SpecularIntensity() const;
		void SetSpecularIntensity(float intensity);

		float SpecularPower() const;
		void SetSpecularPower(float power);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void CreateVertexBuffer(const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;

		inline static const float RotationRate{ DirectX::XM_PI };

		std::shared_ptr<PointLightMaterial> mMaterial;
		DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mColorTexture;
		std::uint32_t mIndexCount{ 0 };
		Library::PointLight mPointLight;
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		float mModelRotationAngle{ 0.0f };
		bool mAnimationEnabled{ true };
		bool mUpdateMaterial{ true };
	};
}