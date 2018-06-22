#pragma once

#include <windows.h>
#include <exception>
#include <string>

namespace Library
{
	class GameException : public std::exception
	{
	public:
		GameException(const char* const message, HRESULT hr = S_OK);

		HRESULT HR() const;
		std::wstring whatw() const;

	private:
		HRESULT mHR;
	};

	inline void ThrowIfFailed(HRESULT hr, const char* const message = "")
	{
		if (FAILED(hr))
		{
			throw GameException(message, hr);
		}
	}

	inline void Assert(bool condition, const wchar_t* const message = L"")
	{
		if (!condition)
		{
			OutputDebugString(message);
			std::terminate();
		}
	}
}