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
	class AmbientLightingDemo final : public Library::DrawableGameComponent
	{
	public:
		AmbientLightingDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		
		float AmbientLightIntensity() const;
		void SetAmbientLightIntensity(float ambientIntensity);

		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		struct CBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			CBufferPerObject() = default;
			CBufferPerObject(const DirectX::XMFLOAT4X4& wvp) : WorldViewProjection(wvp) { }
		};

		struct CBufferPerFrame
		{
			DirectX::XMFLOAT4 AmbientColor{ 1.0f, 1.0f, 1.0f, 1.0f }; // Constant buffers must have a byte width in multiples of 16

			CBufferPerFrame() = default;
			CBufferPerFrame(const DirectX::XMFLOAT4& ambientColor) : AmbientColor(ambientColor) { }
		};

		void CreateVertexBuffer(const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;

		DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		CBufferPerObject mCBufferPerObjectData;
		CBufferPerFrame mCBufferPerFrameData;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mCBufferPerObject;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mCBufferPerFrame;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mColorTexture;		
		std::uint32_t mIndexCount{ 0 };
	};
}
