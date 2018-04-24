#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <gsl\gsl>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"

namespace Library
{
	class Mesh;
	class SkyboxMaterial;

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
		void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;
		void UpdateMaterial();

		std::wstring mCubeMapFileName;
		DirectX::XMFLOAT4X4 mWorldMatrix{ MatrixHelper::Identity };
		float mScale;
		std::shared_ptr<SkyboxMaterial> mMaterial;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		std::uint32_t mIndexCount{ 0 };
		DirectX::XMFLOAT3 mLastPosition;
	};
}