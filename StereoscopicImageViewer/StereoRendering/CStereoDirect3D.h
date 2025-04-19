#ifndef __CSTEREODIRECT3D_H__
#define __CSTEREODIRECT3D_H__

#include <windows.h>
#include <d3d9.h>
#include <tchar.h>
#include <mutex>
#include <chrono>
#include <atomic>

#pragma comment(lib, "d3d9.lib")

class CStereoDirect3D
{
public:
	typedef struct {
		BYTE* DataPtr;
		INT Width;
		INT Height;
		INT Channels;
		LPDIRECT3DSURFACE9 Surface;
		std::mutex* Mutex;
	} ImageData;
private:
	HWND        m_HWnd;
	ImageData   m_Left;
	ImageData   m_Right;
	std::chrono::high_resolution_clock::time_point m_LastTimeMeasuring;
	std::atomic<int> m_FrequencyInHz;
private:
	LPDIRECT3D9 m_D3D;
	std::mutex* m_D3DMutex;
	LPDIRECT3DDEVICE9 m_Device;
	std::mutex* m_DeviceMutex;
private:
	void ReInit(ImageData Left, ImageData Right);
	LPDIRECT3DSURFACE9 CreateSurface(ImageData idat);
public:
	CStereoDirect3D(HWND hWnd);
	~CStereoDirect3D();
	BOOL DrawImageRGB(ImageData Left, ImageData Right);
	BOOL DrawImage(ImageData Left, ImageData Right){ return(DrawImageRGB(Left, Right)); }
	BOOL Blt(bool isLeft);
	INT GetFrequency();
};
#endif // __CSTEREODIRECT3D_H__