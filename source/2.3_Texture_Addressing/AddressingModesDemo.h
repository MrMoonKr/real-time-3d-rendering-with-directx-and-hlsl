#pragma once

#include <map>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"

namespace Library
{
	class KeyboardComponent;
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
		};

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
		bool mUpdateConstantBuffer{ true };
	};
}