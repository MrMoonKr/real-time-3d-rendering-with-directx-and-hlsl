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
#include "VectorHelper.h"

namespace Library
{
	class Mesh;

	class ProxyModel : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ProxyModel, DrawableGameComponent)

	public:
		ProxyModel(Game& game, const std::shared_ptr<Camera>& camera, const std::string& modelFileName, float scale = 1.0f);
		ProxyModel(const ProxyModel&) = delete;
		ProxyModel& operator=(const ProxyModel&) = default;
		ProxyModel(ProxyModel&&) = delete;
		ProxyModel& operator=(ProxyModel&&) = default;
		~ProxyModel() = default;

		const DirectX::XMFLOAT3& Position() const;
		const DirectX::XMFLOAT3& Direction() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR PositionVector() const;
		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		bool& DisplayWireframe();

		void SetPosition(float x, float y, float z);
		void SetPosition(DirectX::FXMVECTOR position);
		void SetPosition(const DirectX::XMFLOAT3& position);

		void ApplyRotation(DirectX::CXMMATRIX transform);
		void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;		
		virtual void Draw(const GameTime& gameTime) override;

	private:
		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			VertexCBufferPerObject() = default;
			VertexCBufferPerObject(const DirectX::XMFLOAT4X4& wvp) : WorldViewProjection(wvp) { }
		};

		void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;

		DirectX::XMFLOAT4X4 mWorldMatrix{ MatrixHelper::Identity };
		DirectX::XMFLOAT4X4 mScaleMatrix{ MatrixHelper::Identity };
		DirectX::XMFLOAT3 mPosition{ Vector3Helper::Zero };
		DirectX::XMFLOAT3 mDirection{ Vector3Helper::Forward };
		DirectX::XMFLOAT3 mUp{ Vector3Helper::Up };
		DirectX::XMFLOAT3 mRight{ Vector3Helper::Right };
		std::string mModelFileName;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;		
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexCBufferPerObject;
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		std::uint32_t mIndexCount{ 0 };
		bool mDisplayWireframe{ true };
	};
}