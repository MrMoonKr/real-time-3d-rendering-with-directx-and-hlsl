#include "pch.h"
#include "ImGuiComponent.h"
#include "imgui_impl_dx11.h"
#include "Game.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(ImGuiComponent)

	ImGuiComponent::ImGuiComponent(Game& game, bool useCustomDraw) :
		DrawableGameComponent(game), mUseCustomDraw(useCustomDraw)
	{
	}

	void ImGuiComponent::Initialize()
	{
		auto getWindow = mGame->GetWindowCallback();
		HWND window = reinterpret_cast<HWND>(getWindow());
		ImGui_ImplDX11_Init(window, mGame->Direct3DDevice(), mGame->Direct3DDeviceContext());
	}

	void ImGuiComponent::Shutdown()
	{
		ImGui_ImplDX11_Shutdown();
	}

	void ImGuiComponent::Draw(const GameTime&)
	{
		ImGui_ImplDX11_NewFrame();

		if (mUseCustomDraw == false)
		{
			for (auto& block : mRenderBlocks)
			{
				(*block)();
			}

			ImGui::Render();
		}
	}

	bool ImGuiComponent::UseCustomDraw() const
	{
		return mUseCustomDraw;
	}

	void ImGuiComponent::SetUseCustomDraw(bool useCustomDraw)
	{
		mUseCustomDraw = useCustomDraw;
	}

	void ImGuiComponent::CustomDraw()
	{
		assert(mUseCustomDraw);
		ImGui::Render();
	}

	const std::vector<std::shared_ptr<ImGuiComponent::RenderBlock>>& ImGuiComponent::RenderBlocks() const
	{
		return mRenderBlocks;
	}

	void ImGuiComponent::AddRenderBlock(std::shared_ptr<RenderBlock> block)
	{
		mRenderBlocks.push_back(block);
	}

	void ImGuiComponent::RemoveRenderBlock(std::shared_ptr<RenderBlock> block)
	{
		mRenderBlocks.erase(find(mRenderBlocks.cbegin(), mRenderBlocks.cend(), block));
	}
}