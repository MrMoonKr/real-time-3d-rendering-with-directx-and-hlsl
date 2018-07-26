#pragma once

#include <DirectXColors.h>
#include <map>
#include "Material.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "SamplerStates.h"

namespace Library
{
	class Texture2D;
}

namespace Rendering
{
	enum class ProjectiveTextureMappingDrawModes
	{
		Basic = 0,
		NoReverseProjection,
		WithDepthMap,
		End
	};

	class ProjectiveTextureMappingMaterial : public Library::Material
	{
		RTTI_DECLARATIONS(ProjectiveTextureMappingMaterial, Library::Material)

	public:
		ProjectiveTextureMappingMaterial(Library::Game& game, std::shared_ptr<Library::Texture2D> colormap, std::shared_ptr<Library::Texture2D> projectedMap, std::shared_ptr<Library::Texture2D> depthMap);
		ProjectiveTextureMappingMaterial(const ProjectiveTextureMappingMaterial&) = default;
		ProjectiveTextureMappingMaterial& operator=(const ProjectiveTextureMappingMaterial&) = default;
		ProjectiveTextureMappingMaterial(ProjectiveTextureMappingMaterial&&) = default;
		ProjectiveTextureMappingMaterial& operator=(ProjectiveTextureMappingMaterial&&) = default;
		virtual ~ProjectiveTextureMappingMaterial() = default;

		ProjectiveTextureMappingDrawModes DrawMode() const;
		const std::string& DrawModeString() const;
		void SetDrawMode(ProjectiveTextureMappingDrawModes drawMode);

		winrt::com_ptr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(winrt::com_ptr<ID3D11SamplerState> samplerState);

		std::shared_ptr<Library::Texture2D> ColorMap() const;
		void SetColorMap(std::shared_ptr<Library::Texture2D> texture);

		std::shared_ptr<Library::Texture2D> ProjectedMap() const;
		void SetProjectedMap(std::shared_ptr<Library::Texture2D> texture);

		std::shared_ptr<Library::Texture2D> DepthMap() const;
		void SetDepthMap(std::shared_ptr<Library::Texture2D> texture);

		const DirectX::XMFLOAT4& AmbientColor() const;
		void SetAmbientColor(const DirectX::XMFLOAT4& color);

		const DirectX::XMFLOAT3& LightPosition() const;
		void SetLightPosition(const DirectX::XMFLOAT3& position);

		const float LightRadius() const;
		void SetLightRadius(float radius);

		const DirectX::XMFLOAT4& LightColor() const;
		void SetLightColor(const DirectX::XMFLOAT4& color);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		void UpdateTransforms(DirectX::FXMMATRIX worldViewProjectionMatrix, DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX projectiveTextureMatrix);
		
	private:
		struct VertexCBufferPerFrame
		{
			DirectX::XMFLOAT3 LightPosition{ Library::Vector3Helper::Zero };
			float LightRadius{ 50.0f };
		};

		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection{ Library::MatrixHelper::Identity };
			DirectX::XMFLOAT4X4 World{ Library::MatrixHelper::Identity };
			DirectX::XMFLOAT4X4 ProjectiveTextureMatrix{ Library::MatrixHelper::Identity };
		};

		struct PixelCBufferPerFrame
		{
			float DepthBias;
			DirectX::XMFLOAT3 CameraPosition{ Library::Vector3Helper::Zero };
			DirectX::XMFLOAT4 AmbientColor{ DirectX::Colors::White };
			DirectX::XMFLOAT3 LightPosition{ 0.0f, 0.0f, 0.0f };
			float Padding;
			DirectX::XMFLOAT4 LightColor{ DirectX::Colors::Black };			
		};

		enum class ProjectiveTextureMappingShaderClasses
		{
			Basic = 0,
			NoReverse,
			DepthMap
		};

		static const std::map<ProjectiveTextureMappingDrawModes, std::string> DrawModeDisplayNames;
		static const std::map<ProjectiveTextureMappingDrawModes, ProjectiveTextureMappingShaderClasses> DrawModeShaderClassMap;

		virtual void BeginDraw() override;

		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerObject;
		winrt::com_ptr<ID3D11Buffer> mPixelCBufferPerFrame;
		VertexCBufferPerFrame mVertexCBufferPerFrameData;
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		PixelCBufferPerFrame mPixelCBufferPerFrameData;
		bool mVertexCBufferPerFrameDataDirty{ true };
		bool mPixelCBufferPerFrameDataDirty{ true };
		std::shared_ptr<Library::Texture2D> mColorMap;
		std::shared_ptr<Library::Texture2D> mProjectedMap;
		std::shared_ptr<Library::Texture2D> mDepthMap;
		winrt::com_ptr<ID3D11SamplerState> mSamplerState{ Library::SamplerStates::TrilinearClamp };
		std::map<ProjectiveTextureMappingShaderClasses, winrt::com_ptr<ID3D11ClassInstance>> mShaderClassInstances;
		ProjectiveTextureMappingDrawModes mDrawMode;
	};
}