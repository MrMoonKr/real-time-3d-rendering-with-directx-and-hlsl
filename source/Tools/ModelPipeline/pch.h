#pragma once

// Standard
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <stdio.h>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// Windows
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <wrl.h>

// DirectX
#include <DirectXMath.h>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Library
#include "Utility.h"
#include "..\Library.Shared\Model.h"
#include "..\Library.Shared\Mesh.h"
#include "..\Library.Shared\ModelMaterial.h"

// Library.Desktop
#include "UtilityWin32.h"