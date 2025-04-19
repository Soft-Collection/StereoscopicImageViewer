#include "stdafx.h"
#include "CStereoDirect3D.h"

CStereoDirect3D::CStereoDirect3D(HWND hWnd)
{
	m_HWnd = NULL;
	m_Left = { 0, };
	m_Right = { 0, };
	m_LastTimeMeasuring = std::chrono::high_resolution_clock::now();
	m_FrequencyInHz.store(0);
	//--------------------------------------------------------
	m_D3DMutex = new std::mutex();
	m_DeviceMutex = new std::mutex();
	m_Left.Mutex = new std::mutex();
	m_Right.Mutex = new std::mutex();
	//--------------------------------------------------------
	m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	//--------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //Enable vsync
	std::unique_lock<std::mutex> lock1(*m_D3DMutex); // Lock the mutex
	m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_Device);
	lock1.unlock();
}
CStereoDirect3D::~CStereoDirect3D()
{
	std::unique_lock<std::mutex> lock1(*m_Left.Mutex); // Lock the mutex
	if (m_Left.Surface) m_Left.Surface->Release();
	lock1.unlock();
	std::unique_lock<std::mutex> lock2(*m_Right.Mutex); // Lock the mutex
	if (m_Right.Surface) m_Right.Surface->Release();
	lock2.unlock();
	std::unique_lock<std::mutex> lock3(*m_DeviceMutex); // Lock the mutex
	if (m_Device) m_Device->Release();
	lock3.unlock();
	std::unique_lock<std::mutex> lock4(*m_D3DMutex); // Lock the mutex
	if (m_D3D) m_D3D->Release();
	lock4.unlock();
	//--------------------------------------------------------
	if (m_Left.Mutex != nullptr)
	{
		delete m_Left.Mutex;
		m_Left.Mutex = nullptr;
	}
	if (m_Right.Mutex != nullptr)
	{
		delete m_Right.Mutex;
		m_Right.Mutex = nullptr;
	}
	if (m_DeviceMutex != nullptr)
	{
		delete m_DeviceMutex;
		m_DeviceMutex = nullptr;
	}
	if (m_D3DMutex != nullptr)
	{
		delete m_D3DMutex;
		m_D3DMutex = nullptr;
	}
}
void CStereoDirect3D::ReInit(ImageData Left, ImageData Right)
{
	std::unique_lock<std::mutex> lock1(*m_Left.Mutex); // Lock the mutex
	if (m_Left.Surface) m_Left.Surface->Release();
	lock1.unlock();
	std::unique_lock<std::mutex> lock2(*m_Right.Mutex); // Lock the mutex
	if (m_Right.Surface) m_Right.Surface->Release();
	lock2.unlock();
}
LPDIRECT3DSURFACE9 CStereoDirect3D::CreateSurface(ImageData idat)
{
	LPDIRECT3DSURFACE9 sysMemSurface = nullptr;
	LPDIRECT3DSURFACE9 videoSurface = nullptr;
	m_Device->CreateOffscreenPlainSurface(idat.Width, idat.Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &sysMemSurface, nullptr);
	D3DLOCKED_RECT rect;
	if (SUCCEEDED(sysMemSurface->LockRect(&rect, nullptr, 0))) {
		DWORD* pixels = (DWORD*)rect.pBits;
		if (idat.Channels == 3)
		{
			for (int y = 0; y < idat.Height; y++) {
				for (int x = 0; x < idat.Width; x++) {
					DWORD pixel = 0;
					DWORD alpha = 255;
					DWORD r = idat.DataPtr[(y * idat.Width + x) * 3 + 0];
					DWORD g = idat.DataPtr[(y * idat.Width + x) * 3 + 1];
					DWORD b = idat.DataPtr[(y * idat.Width + x) * 3 + 2];
					pixel |= (alpha << 24);
					pixel |= (r << 16);
					pixel |= (g << 8);
					pixel |= (b << 0);
					pixels[y * idat.Width + x] = pixel;
				}
			}
		}
		else if (idat.Channels == 4)
		{
			memcpy(pixels, idat.DataPtr, idat.Width * idat.Height * idat.Channels);
		}
		sysMemSurface->UnlockRect();
	}
	m_Device->CreateOffscreenPlainSurface(idat.Width, idat.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &videoSurface, nullptr);
	m_Device->UpdateSurface(sysMemSurface, nullptr, videoSurface, nullptr);
	sysMemSurface->Release();
	return videoSurface;
}
BOOL CStereoDirect3D::DrawImageRGB(ImageData Left, ImageData Right)
{
	ReInit(Left, Right);
	std::unique_lock<std::mutex> lock1(*m_Left.Mutex); // Lock the mutex
	m_Left.Surface = CreateSurface(Left);
	lock1.unlock();
	std::unique_lock<std::mutex> lock2(*m_Right.Mutex); // Lock the mutex
	m_Right.Surface = CreateSurface(Right);
	lock2.unlock();
	return m_Left.Surface && m_Right.Surface;
}
BOOL CStereoDirect3D::Blt(bool isLeft)
{
	LPDIRECT3DSURFACE9 backBuffer = nullptr;
	std::unique_lock<std::mutex> lock1(*m_Left.Mutex); // Lock the mutex
	std::unique_lock<std::mutex> lock2(*m_Right.Mutex); // Lock the mutex
	std::unique_lock<std::mutex> lock3(*m_DeviceMutex); // Lock the mutex
	m_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (backBuffer) {
		m_Device->BeginScene();
		LPDIRECT3DSURFACE9 srcSurface = isLeft ? m_Left.Surface : m_Right.Surface;
		m_Device->StretchRect(srcSurface, nullptr, backBuffer, nullptr, D3DTEXF_NONE);
		m_Device->EndScene();
		m_Device->Present(nullptr, nullptr, nullptr, nullptr); //Blocks until new vsync.
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_LastTimeMeasuring);
		if (duration.count() > 0)
		{
			m_FrequencyInHz.store((INT)(1000000.0 / (double)duration.count()));
		}
		m_LastTimeMeasuring = std::chrono::high_resolution_clock::now();
		backBuffer->Release();
	}
	lock3.unlock();
	lock2.unlock();
	lock1.unlock();
	return TRUE;
}
INT CStereoDirect3D::GetFrequency()
{
	return m_FrequencyInHz.load();
}