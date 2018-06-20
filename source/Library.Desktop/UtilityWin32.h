#pragma once

#include <Windows.h>
#include <string>
#include <functional>
#include <vector>
#include <memory>

namespace Library
{
	class UtilityWin32 final
	{
	public:
		typedef std::function<bool(HWND, UINT, WPARAM, LPARAM)> WndProcHandler;

		static void InitializeWindow(WNDCLASSEX& window, HWND& windowHandle, HINSTANCE instance, const std::wstring& className, const std::wstring& windowTitle, const SIZE& renderTargetSize, int showCommand);
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
		static POINT CenterWindow(const SIZE& windowSize);

		static std::string CurrentDirectory();
		static std::wstring ExecutableDirectory();
		static void PathJoin(std::wstring& dest, const std::wstring& sourceDirectory, const std::wstring& sourceFile);
		static void GetPathExtension(const std::wstring& source, std::wstring& dest);

		static const std::vector<std::shared_ptr<WndProcHandler>>& WndProcHandlers();
		static void AddWndProcHandler(std::shared_ptr<WndProcHandler> handler);
		static void RemoveWndProcHandler(std::shared_ptr<WndProcHandler> handler);

		UtilityWin32() = delete;
		UtilityWin32(const UtilityWin32&) = delete;
		UtilityWin32& operator=(const UtilityWin32&) = delete;
		UtilityWin32(UtilityWin32&&) = delete;
		UtilityWin32& operator=(UtilityWin32&&) = delete;
		~UtilityWin32() = default;

	private:
		static std::vector<std::shared_ptr<WndProcHandler>> sWndProcHandlers;
	};
}