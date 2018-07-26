#pragma once

#include <gsl\gsl>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include "PointLight.h"

namespace Library
{
	class ProxyModel;
	class RenderableFrustum;
}

namespace Rendering
{
	class ProjectiveTextureMappingMaterial;

	class ProjectiveTextureMappingDemo final : public Library::DrawableGameComponent
	{
	public:
		ProjectiveTextureMappingDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		ProjectiveTextureMappingDemo(const ProjectiveTextureMappingDemo&) = delete;
		ProjectiveTextureMappingDemo(ProjectiveTextureMappingDemo&&) = default;
		ProjectiveTextureMappingDemo& operator=(const ProjectiveTextureMappingDemo&) = default;		
		ProjectiveTextureMappingDemo& operator=(ProjectiveTextureMappingDemo&&) = default;
		~ProjectiveTextureMappingDemo();

		float AmbientLightIntensity() const;
		void SetAmbientLightIntensity(float intensity);

		float PointLightIntensity() const;
		void SetPointLightIntensity(float intensity);

		float PointLightRadius() const;
		void SetPointLightRadius(float radius);

		const DirectX::XMFLOAT3& ProjectorPosition() const;
		const DirectX::XMVECTOR ProjectorPositionVector() const;
		void SetProjectorPosition(const DirectX::XMFLOAT3& position);
		void SetProjectorPosition(DirectX::FXMVECTOR position);

		const DirectX::XMFLOAT3& ProjectorDirection() const;
		void RotateProjector(const DirectX::XMFLOAT2& amount);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		void InitializeProjectedTextureScalingMatrix(uint32_t textureWidth, uint32_t textureHeight);
		
		DirectX::XMFLOAT4X4 mPlaneWorldMatrix{ Library::MatrixHelper::Identity };
		DirectX::XMFLOAT4X4 mProjectedTextureScalingMatrix{ Library::MatrixHelper::Zero };
		Library::PointLight mPointLight;
		std::shared_ptr<ProjectiveTextureMappingMaterial> mMaterial;
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		std::uint32_t mVertexCount{ 0 };		
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		std::unique_ptr<Library::Camera> mProjector;
		std::unique_ptr<Library::RenderableFrustum> mRenderableProjectorFrustum;		
		bool mUpdateMaterial{ true };
	};
}