#ifndef __CAUTOMEMORYDIB_H__
#define __CAUTOMEMORYDIB_H__

#include "CAutoMemoryBase.h"

class CAutoMemoryDib : public CAutoMemoryBase
{
private:
	HWND        m_HWnd;
	HDC         m_MemoryDC;
	HBITMAP     m_MemoryBitmap;
	PVOID       m_pvBits; 
	BITMAPINFO  m_bi;
	INT         m_ImageWidth;
	INT         m_ImageHeight;
private:
	void ReInit(HWND hWnd, INT ImageWidth, INT ImageHeight);
public:
	CAutoMemoryDib();
	~CAutoMemoryDib();
	BOOL DrawImageRGB32(HWND hWnd, BYTE* ImageDataPtr, INT ImageWidth, INT ImageHeight);
	BOOL DrawImage(HWND hWnd, BYTE* ImageDataPtr, INT ImageWidth, INT ImageHeight){ return(DrawImageRGB32(hWnd, ImageDataPtr, ImageWidth, ImageHeight)); }
	BOOL Blt();
};
#endif // __CAUTOMEMORYDIB_H__