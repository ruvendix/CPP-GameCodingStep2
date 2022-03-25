#pragma once

#pragma region 표준
#include <cassert>

#include <string>
#include <memory>
#include <vector>
#include <random>
#include <locale>
#include <codecvt>
#include <array>
#include <functional>
#include <unordered_map>
#include <queue>
#pragma endregion

#pragma region Win32API
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <windowsx.h>
#include <comdef.h>
#include <timeapi.h>

#include <wrl/client.h>

#pragma comment(lib, "winmm.lib")
#pragma endregion

#pragma region DirectX11
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma endregion

#pragma region External
#include "DirectXTex/DirectXTex/DirectXTex.h"

#pragma comment(lib, "DirectXTex.lib")
#pragma endregion

#pragma region 프로젝트_전용
#include "CommonInclude/Macro.h"
#include "CommonInclude/Struct.h"
#pragma endregion