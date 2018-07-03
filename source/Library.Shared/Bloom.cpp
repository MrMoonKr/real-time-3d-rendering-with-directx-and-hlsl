#include "pch.h"
#include "Bloom.h"
#include "FullScreenQuadMaterial.h"
#include "Game.h"
#include "VectorHelper.h"
#include "PixelShader.h"
#include "Utility.h"

using namespace std;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace Library
{
    RTTI_DEFINITIONS(Bloom)

	const map<BloomDrawModes, string> Bloom::DrawModeDisplayNames
	{
		{ BloomDrawModes::Normal, "Normal"s },
		{ BloomDrawModes::ExtractedTexture, "Extracted Texture"s },
		{ BloomDrawModes::BlurredTexture, "Blurred Texture"s }
	};

    Bloom::Bloom(Game& game, const BloomSettings& bloomSettings) :
		DrawableGameComponent(game),
		mFullScreenQuad(game), mPixelCBufferPerObjectData(bloomSettings),
		mRenderTarget(game), mGaussianBlur(game, bloomSettings.BlurAmount)
    {
    }

    ID3D11ShaderResourceView* Bloom::SceneTexture() const
    {
        return mSceneTexture.get();
    }

    void Bloom::SetSceneTexture(com_ptr<ID3D11ShaderResourceView> sceneTexture)
    {
		mSceneTexture = sceneTexture;
    }

	BloomSettings Bloom::GetBloomSettings() const
	{
		BloomSettings bloomSettings{
			mPixelCBufferPerObjectData.BloomThreshold,
			mGaussianBlur.BlurAmount(),
			mPixelCBufferPerObjectData.BloomIntensity,
			mPixelCBufferPerObjectData.BloomSaturation,
			mPixelCBufferPerObjectData.SceneIntensity,
			mPixelCBufferPerObjectData.SceneSaturation
		};

		return bloomSettings;
	}

	void Bloom::SetBloomSettings(const BloomSettings& bloomSettings)
	{
		mPixelCBufferPerObjectData = bloomSettings;
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);
		mGaussianBlur.SetBlurAmount(bloomSettings.BlurAmount);
	}

	BloomDrawModes Bloom::DrawMode() const
	{
		return mDrawMode;
	}

	const string& Bloom::DrawModeString() const
	{
		return DrawModeDisplayNames.at(mDrawMode);
	}

	void Bloom::SetDrawMode(BloomDrawModes drawMode)
	{
		mDrawMode = drawMode;
	}

    void Bloom::Initialize()
    {
		mFullScreenQuad.Initialize();
		auto fullScreenQuadMaterial = mFullScreenQuad.Material();
		fullScreenQuadMaterial->SetUpdateMaterialCallback([&]
		{			
			auto psConstantBuffers = mPixelCBufferPerObject.get();
			mGame->Direct3DDeviceContext()->PSSetConstantBuffers(0, 1, &psConstantBuffers);
		});

		mGaussianBlur.SetSceneTexture(mRenderTarget.OutputTexture());
		mGaussianBlur.Initialize();
		
		// Create the bloom shader with class linkage
		auto direct3DDevice = mGame->Direct3DDevice();
		mClassLinkage = Shader::CreateClassLinkage(direct3DDevice);

		auto pixelShaderContentReader = [&](const wstring& assetName)
		{
			com_ptr<ID3D11PixelShader> pixelShader;
			vector<char> compiledPixelShader;
			Utility::LoadBinaryFile(assetName, compiledPixelShader);
			ThrowIfFailed(direct3DDevice->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), mClassLinkage.get(), pixelShader.put()), "ID3D11Device::CreatedPixelShader() failed.");

			return shared_ptr<PixelShader>(new PixelShader(move(pixelShader)));
		};

		mPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\BloomPS.cso", false, pixelShaderContentReader);
		
		fullScreenQuadMaterial->SetPixelShader(mPixelShader);

		ThrowIfFailed(mClassLinkage->CreateClassInstance("ExtractBloomShader", 0, 0, 0, 0, mShaderClassInstances[BloomShaderClass::Extract].put()));
		ThrowIfFailed(mClassLinkage->CreateClassInstance("CompositeBloomShader", 0, 0, 0, 0, mShaderClassInstances[BloomShaderClass::Composite].put()));
		ThrowIfFailed(mClassLinkage->CreateClassInstance("NoBloomShader", 0, 0, 0, 0, mShaderClassInstances[BloomShaderClass::NoBloom].put()));
				
		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerObject);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);

		using namespace std::placeholders;
		mDrawFunctions[BloomDrawModes::Normal] = bind(&Bloom::DrawNormal, this, _1);
		mDrawFunctions[BloomDrawModes::ExtractedTexture] = bind(&Bloom::DrawExtractedTexture, this, _1);
		mDrawFunctions[BloomDrawModes::BlurredTexture] = bind(&Bloom::DrawBlurredTexture, this, _1);
    }

    void Bloom::Draw(const GameTime& gameTime)
    {
		mDrawFunctions.at(mDrawMode)(gameTime);
    }

	void Bloom::DrawNormal(const GameTime& gameTime)
	{
		auto fullScreenQuadMaterial = mFullScreenQuad.Material();
		if (mPixelCBufferPerObjectData.BloomThreshold < 1.0f)
		{
			// Extract the bright spots in the scene
			mRenderTarget.Begin();
			mGame->Direct3DDeviceContext()->ClearRenderTargetView(mRenderTarget.RenderTargetView().get(), Colors::Purple.f);
			mGame->Direct3DDeviceContext()->ClearDepthStencilView(mRenderTarget.DepthStencilView().get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
						
			fullScreenQuadMaterial->SetPixelShaderClassInstance(mShaderClassInstances.at(BloomShaderClass::Extract));
			fullScreenQuadMaterial->SetTexture(mSceneTexture.get());
			
			mFullScreenQuad.Draw(gameTime);
			mRenderTarget.End();
			mGame->UnbindPixelShaderResources<1>();

			// Blur the bright spots in the scene
			mGaussianBlur.DrawToTexture(gameTime);
			
			// Combine the original scene with the blurred bright spot image
			fullScreenQuadMaterial->SetPixelShaderClassInstance(mShaderClassInstances.at(BloomShaderClass::Composite));

			ID3D11ShaderResourceView* psShaderResources[] = { mSceneTexture.get(), mGaussianBlur.OutputTexture() };			
			const span<ID3D11ShaderResourceView*> textures{ psShaderResources };
			fullScreenQuadMaterial->SetTextures(textures);
			mFullScreenQuad.Draw(gameTime);
			mGame->UnbindPixelShaderResources<2>();
		}
		else
		{
			fullScreenQuadMaterial->SetPixelShaderClassInstance(mShaderClassInstances.at(BloomShaderClass::NoBloom));
			fullScreenQuadMaterial->SetTexture(mSceneTexture.get());

			mFullScreenQuad.Draw(gameTime);
			mGame->UnbindPixelShaderResources<2>();
		}
	}

	void Bloom::DrawExtractedTexture(const GameTime& gameTime)
	{
		gameTime;
		/*mFullScreenQuad.SetCustomUpdateMaterial([&]() {
			ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();

			direct3DDeviceContext->PSSetShader(mPixelShader.Get(), mShaderClassInstances[BloomShaderClass::Extract].GetAddressOf(), 1);
			direct3DDeviceContext->PSSetShaderResources(0, 1, mSceneTexture.GetAddressOf());
			direct3DDeviceContext->PSSetSamplers(0, 1, SamplerStates::TrilinearWrap.GetAddressOf());
			direct3DDeviceContext->PSSetConstantBuffers(0, 1, mPixelCBufferPerObject.GetAddressOf());
		});

		mFullScreenQuad.Draw(gameTime);*/
	}

	void Bloom::DrawBlurredTexture(const GameTime& gameTime)
	{
		gameTime;
		// Extract the bright spots in the scene
		/*mRenderTarget.Begin();
		mGame->Direct3DDeviceContext()->ClearRenderTargetView(mRenderTarget.RenderTargetView(), reinterpret_cast<const float*>(&Colors::Purple));
		mGame->Direct3DDeviceContext()->ClearDepthStencilView(mRenderTarget.DepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		mFullScreenQuad.SetCustomUpdateMaterial([&]() {
			ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();

			direct3DDeviceContext->PSSetShader(mPixelShader.Get(), mShaderClassInstances[BloomShaderClass::Extract].GetAddressOf(), 1);
			direct3DDeviceContext->PSSetShaderResources(0, 1, mSceneTexture.GetAddressOf());
			direct3DDeviceContext->PSSetSamplers(0, 1, SamplerStates::TrilinearWrap.GetAddressOf());
			direct3DDeviceContext->PSSetConstantBuffers(0, 1, mPixelCBufferPerObject.GetAddressOf());
		});

		mFullScreenQuad.Draw(gameTime);

		mRenderTarget.End();
		mGame->UnbindPixelShaderResources(0, 1);

		mGaussianBlur->Draw(gameTime);*/
	}
}