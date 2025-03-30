#ifndef __CAUTOMEMORYBASE_H__
#define __CAUTOMEMORYBASE_H__

class CAutoMemoryBase
{
public:
	CAutoMemoryBase(){}
	virtual ~CAutoMemoryBase(){}
	virtual BOOL DrawImage(BYTE* ImageDataPtr, INT ImageWidth, INT ImageHeight) = 0;
	virtual BOOL Blt(HWND hWnd) = 0;
};
#endif // __CAUTOMEMORYBASE_H__