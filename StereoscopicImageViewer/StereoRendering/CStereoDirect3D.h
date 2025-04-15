#ifndef __CSTEREODIRECT3D_H__
#define __CSTEREODIRECT3D_H__

#include <windows.h>
#include <d3d9.h>
#include <tchar.h>

#pragma comment(lib, "d3d9.lib")

class CStereoDirect3D
{
private:
	HWND        m_HWnd;
	INT         m_ImageWidth;
	INT         m_ImageHeight;
private:
	LPDIRECT3D9 mD3D = nullptr;
	LPDIRECT3DDEVICE9 mDevice = nullptr;
	LPDIRECT3DSURFACE9 mLeftSurface = nullptr;
	LPDIRECT3DSURFACE9 mRightSurface = nullptr;
private:
	void ReInit(HWND hWnd, INT ImageWidth, INT ImageHeight);
	LPDIRECT3DSURFACE9 CreateSurface(BYTE* ImageDataPtr, int ImageWidth, int ImageHeight);
public:
	CStereoDirect3D();
	~CStereoDirect3D();
	BOOL DrawImageRGB32(HWND hWnd, BYTE* LeftImageDataPtr, BYTE* RightImageDataPtr, INT ImageWidth, INT ImageHeight);
	BOOL DrawImage(HWND hWnd, BYTE* LeftImageDataPtr, BYTE* RightImageDataPtr, INT ImageWidth, INT ImageHeight){ return(DrawImageRGB32(hWnd, LeftImageDataPtr, RightImageDataPtr, ImageWidth, ImageHeight)); }
	BOOL Blt(bool isLeft);
};
#endif // __CSTEREODIRECT3D_H__