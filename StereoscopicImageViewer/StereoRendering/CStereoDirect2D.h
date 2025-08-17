#ifndef __CSTEREODIRECT2D_H__
#define __CSTEREODIRECT2D_H__

#include "../SerialPort/CComPort.h"
#include "../Imager/CImage.h"
#include <windows.h>
#include <d3d11_1.h>
#include <d2d1_1.h> //For D2D1_BITMAP_OPTIONS_CPU_WRITE
#include <d2d1_3.h> //For ID2D1Factory7 and friends (Windows 10+)
#include <dxgi1_6.h>
#include <wrl.h>
#include <tchar.h>
#include <mutex>
#include <chrono>
#include <atomic>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

typedef void(*dSendSync)(void* user, int syncType);

class CStereoDirect2D
{
private:
	typedef struct {
		INT Width;
		INT Height;
		INT Channels;
	} ImageDimensions;
private:
	HWND               m_HWnd;
	//------------------------------------
	ComPtr<ID2D1Bitmap1> m_LRInBitmap;
	ComPtr<ID2D1Bitmap1> m_LROutBitmap;
	//------------------------------------
	D2D1_RECT_F        m_SourceRect;
	D2D1_RECT_F        m_D2DClientRect;
	ImageDimensions    m_LastImageDimensions;
	CImage*            m_LeftImage;
	CImage*            m_RightImage;
	std::chrono::high_resolution_clock::time_point m_LastTimeMeasuring;
	std::atomic<int>   m_FrequencyInHz;
	std::atomic<int>   m_LRBoth;
	std::atomic<BOOL>  m_SwapLR;
	std::atomic<BOOL>  m_ImageDataUpdated;
private:
	std::mutex*        mMutexDrawBlt;
	//------------------------------------
	ComPtr<ID3D11Device>        m_D3DDevice;
	ComPtr<ID3D11DeviceContext> m_D3DContext;
	ComPtr<IDXGISwapChain1>     m_SwapChain;
	ComPtr<ID2D1Factory7>       m_D2DFactory;
	ComPtr<ID2D1Device5>        m_D2DDevice;
	ComPtr<ID2D1DeviceContext5> m_D2DContext;
	//------------------------------------
private:
	void CreateDevice();
	void InitDevice();
	void InitSwapChain();
	void CreateRenderTarget();
	void CreateSourceBitmap(UINT width, UINT height);
	void ReleaseDevice();
	void ReInitBitmap(ImageDimensions imageDimensions);
	void ResetDevice();
	void SelectSourceRect(BOOL isLeft);
	void DrawOnLRBitmap(CImage* leftImage, CImage* rightImage);
	void ResizeSwapChain(UINT width, UINT height);
	void MeasureFrequencyInHz();
	void ShowErrorInformation(HRESULT hr);
public:
	CStereoDirect2D(HWND hWnd);
	~CStereoDirect2D();
	void DrawImageRGB(CImage* leftImage, CImage* rightImage);
	void DrawImage(CImage* leftImage, CImage* rightImage){ return(DrawImageRGB(leftImage, rightImage)); }
	void Blt(BOOL isLeft, void* user, dSendSync sendSync);
	INT GetFrequency();
	void StereoLRBoth(int lrboth);
	void StereoSwapLR(BOOL swaplr);
	void StereoWindowSizeChanged();
};
#endif // __CSTEREODIRECT2D_H__