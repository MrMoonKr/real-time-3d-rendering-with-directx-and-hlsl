#pragma once

// Standard
#include <exception>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include <map>
#include <stack>
#include <cstdint>
#include <iomanip>
#include <codecvt>
#include <algorithm>
#include <functional>
#include <limits>
#include <filesystem>
#include <tuple>
#include <iterator>
#include <random>
#include <cmath>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// Guidelines Support Library
#include <gsl\gsl>

#define NOMINMAX
// Windows
#include <windows.h>
#include <winrt\Windows.Foundation.h>

// DirectX
#include <d3d11_3.h>
#include <dxgi1_2.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXTK\DDSTextureLoader.h>
#include <DirectXTK\WICTextureLoader.h>
#include <DirectXTK\SpriteBatch.h>
#include <DirectXTK\SpriteFont.h>
#include <DirectXTK\GamePad.h>
#include <DirectXTK\Keyboard.h>
#include <DirectXTK\Mouse.h>