#ifndef __CAUTOMEMORYDIRECT3D_H__
#define __CAUTOMEMORYDIRECT3D_H__

#include "CD3d.hpp"
#include "CAutoMemoryBase.h"

class CAutoMemoryDirect3D : public CAutoMemoryBase
{
private:
	typedef enum _tagSTREAMFORMAT
	{
		NONE	= 0,
		YUY2	= 1,
		YV12	= 2,
		RGB24   = 3,
		RGB32   = 4,
	}eSTREAMFORMAT;
	#define	_D3D_SURFACE_COLORFORMAT	(CD3d::CF_BGRX) //(CD3d::CF_YUY2) //(CD3d::CF_BGR565)
	#define _D3D_FONT_ALPHA				(168)
	#define _D3D_FONT_RGB				(RGB(128,240,240))
	#define _D3D_RECT_RGB				(RGB(240,128,128))
private:
	BYTE*       m_ImageDataPtr;
	INT         m_ImageWidth;
	INT         m_ImageHeight;
	CD3d*       m_cD3d;
private:
	void ReInit(INT ImageWidth, INT ImageHeight);
	HRESULT _D3DDrawString(LPCWSTR Message, INT _nX, INT _nY, DWORD _dwWidth, DWORD _dwHeight);
	HRESULT _D3DDrawCaptureStream(LPBYTE _lpStream, DWORD _dwStreamWidth, DWORD _dwStreamHeight, eSTREAMFORMAT _eStreamFormat);
	BOOL _D3DPrimaryPresentation();
public:
	CAutoMemoryDirect3D();
	~CAutoMemoryDirect3D();
	BOOL DrawImageRGB32(BYTE* ImageDataPtr, INT ImageWidth, INT ImageHeight);
	BOOL DrawImage(BYTE* ImageDataPtr, INT ImageWidth, INT ImageHeight){ return(DrawImageRGB32(ImageDataPtr, ImageWidth, ImageHeight)); }
	BOOL Blt(HWND hWnd);
};
#endif // __CAUTOMEMORYDIRECT3D_H__