#ifndef __CAUTOMEMORYDIRECT3D_H__
#define __CAUTOMEMORYDIRECT3D_H__

#include "CAutoMemoryBase.h"
#include <windows.h>
#include <d3d9.h>
#include <tchar.h>
#include <cassert>
#include <cstring>

// Link the Direct3D 9 library.
#pragma comment(lib, "d3d9.lib")

// Our custom vertex structure with pre-transformed coordinates.
struct CUSTOMVERTEX {
	float x, y, z, rhw; // screen/clip-space position (rhw = 1.0)
	float tu, tv;       // texture coordinates
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

class CAutoMemoryDirect3D : public CAutoMemoryBase
{
private:
	HWND        m_HWnd;
	INT         m_ImageWidth;
	INT         m_ImageHeight;
private:
	LPDIRECT3D9             g_pD3D;
	LPDIRECT3DDEVICE9       g_pd3dDevice;
	LPDIRECT3DTEXTURE9      g_pTexture;
private:
	void ReInit(HWND hWnd, INT ImageWidth, INT ImageHeight);
public:
	CAutoMemoryDirect3D();
	~CAutoMemoryDirect3D();
	BOOL DrawImageRGB32(HWND hWnd, BYTE* ImageDataPtr, INT ImageWidth, INT ImageHeight);
	BOOL DrawImage(HWND hWnd, BYTE* ImageDataPtr, INT ImageWidth, INT ImageHeight){ return(DrawImageRGB32(hWnd, ImageDataPtr, ImageWidth, ImageHeight)); }
	BOOL Blt();
};
#endif // __CAUTOMEMORYDIRECT3D_H__