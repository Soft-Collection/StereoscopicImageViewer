#ifndef __CAUTOMEMORYDIB_H__
#define __CAUTOMEMORYDIB_H__

#include "CAutoMemoryBase.h"

class CAutoMemoryDib : public CAutoMemoryBase
{
private:
	HDC         m_MemoryDC;
	HBITMAP     m_MemoryBitmap;
	PVOID       m_pvBits; 
	BITMAPINFO  m_bi;
	INT         m_ImageWidth;
	INT         m_ImageHeight;
private:
	void ReInit(INT ImageWidth, INT ImageHeight);
public:
	CAutoMemoryDib();
	~CAutoMemoryDib();
	BOOL DrawImageRGB32(BYTE* ImageDataPtr, INT ImageWidth, INT ImageHeight);
	BOOL DrawImage(BYTE* ImageDataPtr, INT ImageWidth, INT ImageHeight){ return(DrawImageRGB32(ImageDataPtr, ImageWidth, ImageHeight)); }
	BOOL Blt(HWND hWnd);
};
#endif // __CAUTOMEMORYDIB_H__