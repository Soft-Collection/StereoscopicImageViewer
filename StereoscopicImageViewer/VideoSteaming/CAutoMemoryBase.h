#ifndef __CAUTOMEMORYBASE_H__
#define __CAUTOMEMORYBASE_H__

class CAutoMemoryBase
{
protected:
	void ReInit(HWND hWnd, INT ImageWidth, INT ImageHeight);
public:
	CAutoMemoryBase(){}
	virtual ~CAutoMemoryBase(){}
	virtual BOOL DrawImage(HWND hWnd, BYTE* ImageDataPtr, INT ImageWidth, INT ImageHeight) = 0;
	virtual BOOL Blt() = 0;
};
#endif // __CAUTOMEMORYBASE_H__