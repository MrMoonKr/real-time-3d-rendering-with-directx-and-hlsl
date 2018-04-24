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
	class DirectionalLight;
	class ProxyModel;
}

namespace Rendering
{
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
		void SetAmbientLightIntensity(float ambientIntensity);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection{ Library::MatrixHelper::Identity };
			DirectX::XMFLOAT4X4 World{ Library::MatrixHelper::Identity };

			VertexCBufferPerObject() = default;
			VertexCBufferPerObject(const DirectX::XMFLOAT4X4& wvp, const DirectX::XMFLOAT4X4& world) : WorldViewProjection(wvp), World(world) { }
		};

		struct PixelCBufferPerFrame
		{
			DirectX::XMFLOAT4 AmbientColor{ DirectX::Colors::Black };
			DirectX::XMFLOAT3 LightDirection{ 0.0f, 0.0f, 1.0f };
			float Padding;
			DirectX::XMFLOAT4 LightColor{ DirectX::Colors::White };

			PixelCBufferPerFrame() = default;
			PixelCBufferPerFrame(const DirectX::XMFLOAT4& ambientColor, const DirectX::XMFLOAT3& lightDirection, const DirectX::XMFLOAT4& lightColor) :
				AmbientColor(ambientColor), LightDirection(lightDirection), LightColor(lightColor) { }
		};

		void CreateVertexBuffer(const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;

		inline static const float RotationRate{ DirectX::XM_PI };

		DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		PixelCBufferPerFrame mPixelCBufferPerFrameData;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexCBufferPerObject;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mPixelCBufferPerFrame;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mColorTexture;		
		std::uint32_t mIndexCount{ 0 };
		bool mAnimationEnabled{ true };
		std::unique_ptr<Library::DirectionalLight> mDirectionalLight;
		std::unique_ptr<Library::ProxyModel> mProxyModel;
	};
}