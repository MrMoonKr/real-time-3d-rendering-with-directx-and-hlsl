#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <d3d11_4.h>
#include <DirectXMath.h>
#include <gsl\gsl>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"

namespace Library
{
	class Mesh;

	class Skybox final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Skybox, DrawableGameComponent)

	public:
		Skybox(Game& game, const std::shared_ptr<Camera>& camera, const std::wstring& cubeMapFileName, float scale);
		Skybox(const Skybox&) = delete;
		Skybox(Skybox&&) = default;
		Skybox& operator=(const Skybox&) = delete;		
		Skybox& operator=(Skybox&&) = default;
		~Skybox() = default;

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			VertexCBufferPerObject() { }
			VertexCBufferPerObject(const DirectX::XMFLOAT4X4& wvp) : WorldViewProjection(wvp) { }
		};

		void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;

		DirectX::XMFLOAT4X4 mWorldMatrix{ MatrixHelper::Identity };
		DirectX::XMFLOAT4X4 mScaleMatrix{ MatrixHelper::Identity };
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		std::wstring mCubeMapFileName;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexCBufferPerObject;		
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSkyboxTexture;
		std::uint32_t mIndexCount{ 0 };
	};
}