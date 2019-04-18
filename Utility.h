#pragma once
#ifndef UTILITY_H
#define UTILITY_H


#include <windows.h>
#include <ctime>
#include <mmsystem.h>
#include <crtdbg.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <iostream>
#include <string>
#include <vector>


#pragma comment(lib,"winmm.lib")

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#define WINDOW_TITLE	L"Test"

extern bool Paused;

//===============================================================
// Math Constants

const float _INFINITY = FLT_MAX;
const float EPSILON = 0.001f;

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                      \
	{                                                  \
		HRESULT hr = x;                                \
		if(FAILED(hr))                                 \
		{                                              \
			DXTraceA(__FILE__, __LINE__, hr, #x, TRUE); \
		}                                              \
	}
#endif

#else
#ifndef HR
#define HR(x) x;
#endif
#endif 

//===============================================================
//Set the common colors
const D3DXCOLOR      WHITE(D3DCOLOR_XRGB(255, 255, 255));
const D3DXCOLOR      BLACK(D3DCOLOR_XRGB(0, 0, 0));
const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));
const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));
const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));
const D3DXCOLOR     YELLOW(D3DCOLOR_XRGB(255, 255, 0));
const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));
const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

const D3DXCOLOR BEACH_SAND(D3DCOLOR_XRGB(255, 249, 157));
const D3DXCOLOR DESERT_SAND(D3DCOLOR_XRGB(250, 205, 135));

const D3DXCOLOR LIGHTGREEN(D3DCOLOR_XRGB(60, 184, 120));
const D3DXCOLOR  PUREGREEN(D3DCOLOR_XRGB(0, 166, 81));
const D3DXCOLOR  DARKGREEN(D3DCOLOR_XRGB(0, 114, 54));

const D3DXCOLOR LIGHT_YELLOW_GREEN(D3DCOLOR_XRGB(124, 197, 118));
const D3DXCOLOR  PURE_YELLOW_GREEN(D3DCOLOR_XRGB(57, 181, 74));
const D3DXCOLOR  DARK_YELLOW_GREEN(D3DCOLOR_XRGB(25, 123, 48));

const D3DXCOLOR LIGHTBROWN(D3DCOLOR_XRGB(198, 156, 109));
const D3DXCOLOR DARKBROWN(D3DCOLOR_XRGB(115, 100, 87));


struct Mtrl
{
	Mtrl()
		:ambient(WHITE), diffuse(WHITE), spec(WHITE), specPower(8.0f) {}
	Mtrl(const D3DXCOLOR& a, const D3DXCOLOR& d,
		const D3DXCOLOR& s, float power)
		:ambient(a), diffuse(d), spec(s), specPower(power) {}

	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	float specPower;
};

struct DirLight
{
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	D3DXVECTOR3 dirW;
};

struct AABB
{
	// Initialize to an infinitely small bounding box.
	AABB()
		: minPt(_INFINITY, _INFINITY, _INFINITY),
		maxPt(-_INFINITY, -_INFINITY, -_INFINITY) {}

	D3DXVECTOR3 center()const
	{
		return (minPt + maxPt)*0.5f;
	}

	D3DXVECTOR3 extent()const
	{
		return (maxPt - minPt)*0.5f;
	}

	void xform(const D3DXMATRIX& M, AABB& out)
	{
		// Convert to center/extent representation.
		D3DXVECTOR3 c = center();
		D3DXVECTOR3 e = extent();

		// Transform center in usual way.
		D3DXVec3TransformCoord(&c, &c, &M);

		// Transform extent.
		D3DXMATRIX absM;
		D3DXMatrixIdentity(&absM);
		absM(0, 0) = fabsf(M(0, 0)); absM(0, 1) = fabsf(M(0, 1)); absM(0, 2) = fabsf(M(0, 2));
		absM(1, 0) = fabsf(M(1, 0)); absM(1, 1) = fabsf(M(1, 1)); absM(1, 2) = fabsf(M(1, 2));
		absM(2, 0) = fabsf(M(2, 0)); absM(2, 1) = fabsf(M(2, 1)); absM(2, 2) = fabsf(M(2, 2));
		D3DXVec3TransformNormal(&e, &e, &absM);

		// Convert back to AABB representation.
		out.minPt = c - e;
		out.maxPt = c + e;
	}

	D3DXVECTOR3 minPt;
	D3DXVECTOR3 maxPt;
};

#endif // !UTILITY_H
