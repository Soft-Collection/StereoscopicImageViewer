#ifndef __CSTEREODIRECT3D_H__
#define __CSTEREODIRECT3D_H__

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

class CStereoDirect3D
{
private:
	HWND        m_HWnd;
	INT         m_ImageWidth;
	INT         m_ImageHeight;
private:
	LPDIRECT3D9             g_pD3D;
	LPDIRECT3DDEVICE9       g_pd3dDevice;
	LPDIRECT3DTEXTURE9      g_pLeftTexture;
	LPDIRECT3DTEXTURE9      g_pRightTexture;
private:
	void ReInit(HWND hWnd, INT ImageWidth, INT ImageHeight);
public:
	CStereoDirect3D();
	~CStereoDirect3D();
	BOOL DrawImageRGB32(HWND hWnd, BYTE* LeftImageDataPtr, BYTE* RightImageDataPtr, INT ImageWidth, INT ImageHeight);
	BOOL DrawImage(HWND hWnd, BYTE* LeftImageDataPtr, BYTE* RightImageDataPtr, INT ImageWidth, INT ImageHeight){ return(DrawImageRGB32(hWnd, LeftImageDataPtr, RightImageDataPtr, ImageWidth, ImageHeight)); }
	BOOL Blt(bool isLeft);
};
#endif // __CSTEREODIRECT3D_H__