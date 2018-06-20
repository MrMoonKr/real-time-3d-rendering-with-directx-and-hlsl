#include "pch.h"
#include "RenderingGame.h"
#include "GameException.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "GamePadComponent.h"
#include "FpsComponent.h"
#include "SpotLightDemo.h"
#include "Grid.h"
#include "FirstPersonCamera.h"
#include "SamplerStates.h"
#include "RasterizerStates.h"
#include "VectorHelper.h"
#include "ImGuiComponent.h"
#include "imgui_impl_dx11.h"
#include "UtilityWin32.h"
#include <limits>

using namespace std;
using namespace DirectX;
using namespace Library;

IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Rendering
{
	RenderingGame::RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback)
	{
	}

	void RenderingGame::Initialize()
	{
		SamplerStates::Initialize(Direct3DDevice()); 
		RasterizerStates::Initialize(Direct3DDevice());

		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mMouse = make_shared<MouseComponent>(*this, MouseModes::Absolute);
		mComponents.push_back(mMouse);
		mServices.AddService(MouseComponent::TypeIdClass(), mMouse.get());

		mGamePad = make_shared<GamePadComponent>(*this);
		mComponents.push_back(mGamePad);
		mServices.AddService(GamePadComponent::TypeIdClass(), mGamePad.get());

		auto camera = make_shared<FirstPersonCamera>(*this);
		mComponents.push_back(camera);
		mServices.AddService(Camera::TypeIdClass(), camera.get());

		mGrid = make_shared<Grid>(*this, camera);
		mComponents.push_back(mGrid);

		mSpotLightDemo = make_shared<SpotLightDemo>(*this, camera);
		mComponents.push_back(mSpotLightDemo);

		auto imGui = make_shared<ImGuiComponent>(*this);
		mComponents.push_back(imGui);
		mServices.AddService(ImGuiComponent::TypeIdClass(), imGui.get());
		auto imGuiWndProcHandler = make_shared<UtilityWin32::WndProcHandler>(ImGui_ImplWin32_WndProcHandler);
		UtilityWin32::AddWndProcHandler(imGuiWndProcHandler);

		auto helpTextImGuiRenderBlock = make_shared<ImGuiComponent::RenderBlock>([this]()
		{
			ImGui::Begin("Controls");
			ImGui::SetNextWindowPos(ImVec2(10, 10));

			{
				stringstream fpsLabel;
				fpsLabel << setprecision(3) << "Frame Rate: " << mFpsComponent->FrameRate() << "    Total Elapsed Time: " << mGameTime.TotalGameTimeSeconds().count();
				ImGui::Text(fpsLabel.str().c_str());
			}

			ImGui::Text("Camera (WASD + Left-Click-Mouse-Look)");
			ImGui::Text("Move Spot Light (Num-Pad 8/2, 4/6, 3/9)");			

			{
				stringstream gridVisibleLabel;
				gridVisibleLabel << "Toggle Grid (G): " << (mGrid->Visible() ? "Visible" : "Not Visible");
				ImGui::Text(gridVisibleLabel.str().c_str());
			}
			{
				stringstream ambientLightIntensityLabel;
				ambientLightIntensityLabel << setprecision(2) << "Ambient Light Intensity (+PgUp/-PgDown): " << mSpotLightDemo->AmbientLightIntensity();
				ImGui::Text(ambientLightIntensityLabel.str().c_str());
			}
			{
				stringstream directionalLightIntensityLabel;
				directionalLightIntensityLabel << setprecision(2) << "Spot Light Intensity (+Home/-End): " << mSpotLightDemo->SpotLightIntensity();
				ImGui::Text(directionalLightIntensityLabel.str().c_str());
			}
			{
				stringstream specularIntensityLabel;
				specularIntensityLabel << setprecision(2) << "Specular Intensity (+Insert/-Delete): " << mSpotLightDemo->SpecularIntensity();
				ImGui::Text(specularIntensityLabel.str().c_str());
			}
			{
				stringstream specularPowerLabel;
				specularPowerLabel << "Specular Power (+O/-P): " << mSpotLightDemo->SpecularPower();
				ImGui::Text(specularPowerLabel.str().c_str());
			}
			{
				stringstream spotLightRadiusLabel;
				spotLightRadiusLabel << "Spot Light Radius (+B/-N): " << mSpotLightDemo->LightRadius();
				ImGui::Text(spotLightRadiusLabel.str().c_str());
			}
			{
				stringstream spotLightInnerAngleLabel;
				spotLightInnerAngleLabel << "Spot Light Inner Angle(+Z/-X): " << mSpotLightDemo->SpotLightInnerAngle();
				ImGui::Text(spotLightInnerAngleLabel.str().c_str());
			}
			{
				stringstream spotLightOuterAngleLabel;
				spotLightOuterAngleLabel << "Spot Light Outer Angle(+C/-V): " << mSpotLightDemo->SpotLightOuterAngle();
				ImGui::Text(spotLightOuterAngleLabel.str().c_str());
			}

			ImGui::End();
		});
		imGui->AddRenderBlock(helpTextImGuiRenderBlock);

		mFpsComponent = make_shared<FpsComponent>(*this);
		mFpsComponent->SetVisible(false);
		mComponents.push_back(mFpsComponent);

		Game::Initialize();
		
		camera->SetPosition(0.0f, 5.0f, 20.0f);
		mAmbientLightIntensity = mSpotLightDemo->AmbientLightIntensity();
		mSpotLightIntensity = mSpotLightDemo->SpotLightIntensity();
		mSpotLightInnerAngle = mSpotLightDemo->SpotLightInnerAngle();
		mSpotLightOuterAngle = mSpotLightDemo->SpotLightOuterAngle();
		mSpecularIntensity = mSpotLightDemo->SpecularIntensity();
		mSpecularPower = mSpotLightDemo->SpecularPower();	
	}

	void RenderingGame::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape) || mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back))
		{
			Exit();
		}

		if (mMouse->WasButtonPressedThisFrame(MouseButtons::Left))
		{
			mMouse->SetMode(MouseModes::Relative);
		}

		if (mMouse->WasButtonReleasedThisFrame(MouseButtons::Left))
		{
			mMouse->SetMode(MouseModes::Absolute);
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::G))
		{
			mGrid->SetVisible(!mGrid->Visible());
		}

		UpdateAmbientLightIntensity(gameTime);
		UpdateSpotLight(gameTime);
		UpdateSpecularLight(gameTime);

		Game::Update(gameTime);
	}

	void RenderingGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.get(), BackgroundColor.f);
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

		HRESULT hr = mSwapChain->Present(1, 0);

		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
		}
		else
		{
			ThrowIfFailed(hr, "IDXGISwapChain::Present() failed.");
		}
	}

	void RenderingGame::Shutdown()
	{
		Game::Shutdown();
		RasterizerStates::Shutdown();
		SamplerStates::Shutdown();
	}

	void RenderingGame::Exit()
	{
		PostQuitMessage(0);
	}

	void RenderingGame::UpdateAmbientLightIntensity(const GameTime& gameTime)
	{
		if (mKeyboard->IsKeyDown(Keys::PageUp) && mAmbientLightIntensity < 1.0f)
		{
			mAmbientLightIntensity += gameTime.ElapsedGameTimeSeconds().count();
			mAmbientLightIntensity = min(mAmbientLightIntensity, 1.0f);
			mSpotLightDemo->SetAmbientLightIntensity(mAmbientLightIntensity);
		}
		else if (mKeyboard->IsKeyDown(Keys::PageDown) && mAmbientLightIntensity > 0.0f)
		{
			mAmbientLightIntensity -= gameTime.ElapsedGameTimeSeconds().count();
			mAmbientLightIntensity = max(mAmbientLightIntensity, 0.0f);
			mSpotLightDemo->SetAmbientLightIntensity(mAmbientLightIntensity);
		}
	}

	void RenderingGame::UpdateSpotLight(const GameTime& gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		// Update light intensity
		if (mKeyboard->IsKeyDown(Keys::Home) && mSpotLightIntensity < 1.0f)
		{
			mSpotLightIntensity += elapsedTime;
			mSpotLightIntensity = min(mSpotLightIntensity, 1.0f);
			mSpotLightDemo->SetSpotLightIntensity(mSpotLightIntensity);
		}
		else if (mKeyboard->IsKeyDown(Keys::End) && mSpotLightIntensity > 0.0f)
		{
			mSpotLightIntensity -= elapsedTime;
			mSpotLightIntensity = max(mSpotLightIntensity, 0.0f);
			mSpotLightDemo->SetSpotLightIntensity(mSpotLightIntensity);
		}

		// Move light
		XMFLOAT3 movementAmount = Vector3Helper::Zero;
		if (mKeyboard->IsKeyDown(Keys::NumPad4))
		{
			movementAmount.x = -1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::NumPad6))
		{
			movementAmount.x = 1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::NumPad9))
		{
			movementAmount.y = 1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::NumPad3))
		{
			movementAmount.y = -1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::NumPad8))
		{
			movementAmount.z = -1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::NumPad2))
		{
			movementAmount.z = 1.0f;
		}

		const float LightMovementRate = 10.0f;
		const float LightModulationRate = static_cast<float>(numeric_limits<uint8_t>::max());
		if (movementAmount.x != 0.0f || movementAmount.y != 0.0f || movementAmount.z != 0.0f)
		{
			XMVECTOR movement = XMLoadFloat3(&movementAmount) * LightMovementRate * elapsedTime;
			mSpotLightDemo->SetLightPosition(mSpotLightDemo->LightPositionVector() + movement);
		}

		// Rotate light
		XMFLOAT2 rotationAmount = Vector2Helper::Zero;
		if (mKeyboard->IsKeyDown(Keys::Left))
		{
			rotationAmount.x += LightRotationRate.x * elapsedTime;
		}
		if (mKeyboard->IsKeyDown(Keys::Right))
		{
			rotationAmount.x -= LightRotationRate.x * elapsedTime;
		}
		if (mKeyboard->IsKeyDown(Keys::Up))
		{
			rotationAmount.y += LightRotationRate.y * elapsedTime;
		}
		if (mKeyboard->IsKeyDown(Keys::Down))
		{
			rotationAmount.y -= LightRotationRate.y * elapsedTime;
		}

		if (rotationAmount.x != 0.0f || rotationAmount.y != 0.0f)
		{
			mSpotLightDemo->RotateSpotLight(rotationAmount);
		}

		// Update the light's radius
		if (mKeyboard->IsKeyDown(Keys::B))
		{
			float radius = mSpotLightDemo->LightRadius() + LightModulationRate * elapsedTime;
			mSpotLightDemo->SetLightRadius(radius);
		}
		else if (mKeyboard->IsKeyDown(Keys::N))
		{
			float radius = mSpotLightDemo->LightRadius() - LightModulationRate * elapsedTime;
			radius = max(radius, 0.0f);
			mSpotLightDemo->SetLightRadius(radius);
		}

		// Update inner and outer angles		
		if (mKeyboard->IsKeyDown(Keys::Z) && mSpotLightInnerAngle < 1.0f)
		{
			mSpotLightInnerAngle += elapsedTime;
			mSpotLightInnerAngle = min(mSpotLightInnerAngle, 1.0f);

			mSpotLightDemo->SetSpotLightInnerAngle(mSpotLightInnerAngle);
		}
		else if (mKeyboard->IsKeyDown(Keys::X) && mSpotLightInnerAngle > 0.5f)
		{
			mSpotLightInnerAngle -= elapsedTime;
			mSpotLightInnerAngle = max(mSpotLightInnerAngle, 0.5f);

			mSpotLightDemo->SetSpotLightInnerAngle(mSpotLightInnerAngle);
		}

		if (mKeyboard->IsKeyDown(Keys::C) && mSpotLightOuterAngle < 0.5f)
		{
			mSpotLightOuterAngle += elapsedTime;
			mSpotLightOuterAngle = min(mSpotLightOuterAngle, 0.5f);

			mSpotLightDemo->SetSpotLightOuterAngle(mSpotLightOuterAngle);
		}
		else if (mKeyboard->IsKeyDown(Keys::V) && mSpotLightOuterAngle > 0.0f)
		{
			mSpotLightOuterAngle -= elapsedTime;
			mSpotLightOuterAngle = max(mSpotLightOuterAngle, 0.0f);
			
			mSpotLightDemo->SetSpotLightOuterAngle(mSpotLightOuterAngle);
		}
	}

	void RenderingGame::UpdateSpecularLight(const Library::GameTime & gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		if (mKeyboard->IsKeyDown(Keys::Insert) && mSpecularIntensity < 1.0f)
		{
			mSpecularIntensity += elapsedTime;
			mSpecularIntensity = min(mSpecularIntensity, 1.0f);
			mSpotLightDemo->SetSpecularIntensity(mSpecularIntensity);
		}
		else if (mKeyboard->IsKeyDown(Keys::Delete) && mSpecularIntensity > 0.0f)
		{
			mSpecularIntensity -= elapsedTime;
			mSpecularIntensity = max(mSpecularIntensity, 0.0f);
			mSpotLightDemo->SetSpecularIntensity(mSpecularIntensity);
		}

		const auto ModulationRate = numeric_limits<uint8_t>::max();
		if (mKeyboard->IsKeyDown(Keys::O) && mSpecularPower < numeric_limits<uint8_t>::max())
		{
			mSpecularPower += ModulationRate * elapsedTime;
			mSpecularPower = min(mSpecularPower, static_cast<float>(numeric_limits<uint8_t>::max()));
			mSpotLightDemo->SetSpecularPower(mSpecularPower);
		}
		else if (mKeyboard->IsKeyDown(Keys::P) && mSpecularPower > 1.0f)
		{
			mSpecularPower -= ModulationRate * elapsedTime;
			mSpecularPower = max(mSpecularPower, 1.0f);
			mSpotLightDemo->SetSpecularPower(mSpecularPower);
		}
	}
}