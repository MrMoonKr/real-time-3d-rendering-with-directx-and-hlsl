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
	enum class FilteringModes
	{
		Point = 0,
		TriLinear,
		Anisotropic,
		End
	};

	class FilteringModesDemo final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(FilteringModesDemo, Library::DrawableGameComponent)

	public:
		FilteringModesDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		FilteringModes ActiveFilteringMode() const;

		static const std::map<FilteringModes, std::string> FilteringModeNames;

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
		std::map<FilteringModes, Microsoft::WRL::ComPtr<ID3D11SamplerState>> mTextureSamplersByFilteringMode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mCBufferPerObject;				
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mColorTexture;
		std::uint32_t mIndexCount{ 0 };
		FilteringModes mActiveFilteringMode{ FilteringModes::Point };
		Library::KeyboardComponent* mKeyboard;
	};
}