#pragma once

#include <map>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"

namespace Library
{
	class KeyboardComponent;
	struct VertexPositionTexture;
}

namespace Rendering
{
	enum class AddressingModes
	{
		Wrap = 0,
		Mirror,
		Clamp,
		Border,
		End
	};

	class AddressingModesDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(AddressingModesDemo, Library::DrawableGameComponent)

	public:
		AddressingModesDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		AddressingModes ActiveAddressingMode() const;

		static const std::map<AddressingModes, std::string> AddressingModeNames;

	private:
		struct CBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			CBufferPerObject() = default;
			CBufferPerObject(const DirectX::XMFLOAT4X4& wvp) : WorldViewProjection(wvp) { }
		};
	
		void CreateVertexBuffer(gsl::not_null<Library::VertexPositionTexture*> vertices, std::uint32_t vertexCount, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;
		void CreateIndexBuffer(gsl::not_null<uint32_t*> indices, uint32_t indexCount, gsl::not_null<ID3D11Buffer**> indexBuffer) const;

		DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		CBufferPerObject mCBufferPerObjectData;
		std::map<AddressingModes, Microsoft::WRL::ComPtr<ID3D11SamplerState>> mTextureSamplersByAddressingMode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mCBufferPerObject;				
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mColorTexture;
		std::uint32_t mIndexCount{ 0 };
		AddressingModes mActiveAddressingMode{ AddressingModes::Wrap };
		Library::KeyboardComponent* mKeyboard;
	};
}