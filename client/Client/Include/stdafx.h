// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
//#include <D3D11_1.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

// STL -----------------------------
#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <functional>

using namespace  std;


// Etc -------------------------------
#include <time.h>


// FBX --------------
#include <fbxsdk.h>
#pragma  comment(lib, "libfbxsdk-md.lib")


// Text ----------
#pragma  comment(lib, "FW1FontWrapper.lib")


// D3DX -----------------------------
#include <D3D11.h>
#include <D3DX11.h>

#include <D3DX10.h>
#include <D3D10.h>

#include <d3dcompiler.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


#pragma  comment(lib, "d3dx11.lib")
#pragma  comment(lib, "d3d11.lib")

#pragma  comment(lib, "d3dx10.lib")
#pragma  comment(lib, "d3d10.lib")

#pragma  comment(lib, "D3DCompiler.lib")
#pragma  comment(lib, "dxerr.lib")
#pragma  comment(lib, "dxgi.lib")

#pragma  comment(lib, "dinput8.lib")
#pragma  comment(lib, "dxguid.lib")

#pragma  comment(lib, "d2d1.lib")
#pragma  comment(lib, "dwrite.lib")

#ifdef _DEBUG
	#pragma  comment(lib, "d3dx11d.lib")
	#pragma  comment(lib, "Effects11d.lib")
#else
	#pragma  comment(lib, "d3dx11.lib")
	#pragma  comment(lib, "Effects11d.lib")
#endif


// Debuging -----------------------------
#ifdef _DEBUG
	#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
	extern DWORD		g_dwClientDebugMode;
#endif

#include <crtdbg.h>


// Waring -----------------------------
#pragma warning(disable : 4244) 
#pragma warning(disable : 4305)
#pragma warning(disable : 4996)


// Mgr ----------------------------------
#include "Device.h"
#include "Timer.h"

// Server ---------------------------------
#include "Protocol.h"
