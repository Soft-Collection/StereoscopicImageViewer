#include "stdafx.h"
#include "CStereoDirect3D.h"
#include "../Common/CExceptionReport.h"

CStereoDirect3D::CStereoDirect3D()
{
	try
	{
		m_HWnd = NULL;
		m_ImageWidth = 0;
		m_ImageHeight = 0;
		//--------------------------------------------------------
		mD3D = Direct3DCreate9(D3D_SDK_VERSION);
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CStereoDirect3D::~CStereoDirect3D", "Exception in CStereoDirect3D Constructor");
	}
}
CStereoDirect3D::~CStereoDirect3D()
{
	try
	{
		if (mLeftSurface) mLeftSurface->Release();
		if (mRightSurface) mRightSurface->Release();
		if (mDevice) mDevice->Release();
		if (mD3D) mD3D->Release();
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CStereoDirect3D::~CStereoDirect3D", "Exception in CStereoDirect3D Destructor");
	}
}
void CStereoDirect3D::ReInit(HWND hWnd, int ImageWidth, int ImageHeight)
{
	try
	{
		if ((m_HWnd == hWnd) && (m_ImageWidth == ImageWidth) && (m_ImageHeight == ImageHeight)) return;
		//--------------------------------------------------------
		if (mLeftSurface) mLeftSurface->Release();
		if (mRightSurface) mRightSurface->Release();
		if (mDevice) mDevice->Release();
		//--------------------------------------------------------
		D3DPRESENT_PARAMETERS d3dpp = {};
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //Enable vsync
		mD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &mDevice);
		//--------------------------------------------------------
		m_HWnd = hWnd;
		m_ImageWidth = ImageWidth;
		m_ImageHeight = ImageHeight;
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CStereoDirect3D::ReInit", "Exception in CStereoDirect3D ReInit");
	}		
}
LPDIRECT3DSURFACE9 CStereoDirect3D::CreateSurface(BYTE* ImageDataPtr, int ImageWidth, int ImageHeight)
{
	try
	{
		LPDIRECT3DSURFACE9 sysMemSurface = nullptr;
		LPDIRECT3DSURFACE9 videoSurface = nullptr;
		mDevice->CreateOffscreenPlainSurface(ImageWidth, ImageHeight, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &sysMemSurface, nullptr);
		D3DLOCKED_RECT rect;
		if (SUCCEEDED(sysMemSurface->LockRect(&rect, nullptr, 0))) {
			DWORD* pixels = (DWORD*)rect.pBits;
			memcpy(pixels, ImageDataPtr, ImageWidth * ImageHeight * 4);
			sysMemSurface->UnlockRect();
		}
		mDevice->CreateOffscreenPlainSurface(ImageWidth, ImageHeight, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &videoSurface, nullptr);
		mDevice->UpdateSurface(sysMemSurface, nullptr, videoSurface, nullptr);
		sysMemSurface->Release();
		return videoSurface;
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CStereoDirect3D::ReInit", "Exception in CStereoDirect3D CreateSurface");
	}
}
BOOL CStereoDirect3D::DrawImageRGB32(HWND hWnd, BYTE* LeftImageDataPtr, BYTE* RightImageDataPtr, int ImageWidth, int ImageHeight)
{
	try
	{
		ReInit(hWnd, ImageWidth, ImageHeight);
		mLeftSurface = CreateSurface(LeftImageDataPtr, ImageWidth, ImageHeight);
		mRightSurface = CreateSurface(RightImageDataPtr, ImageWidth, ImageHeight);
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CStereoDirect3D::DrawImageRGB32", "Exception in CStereoDirect3D DrawImageRGB32");
	}
	return mLeftSurface && mRightSurface;
}
BOOL CStereoDirect3D::Blt(bool isLeft)
{
	try
	{
		LPDIRECT3DSURFACE9 backBuffer = nullptr;
		mDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
		if (backBuffer) {
			mDevice->BeginScene();
			LPDIRECT3DSURFACE9 srcSurface = isLeft ? mLeftSurface : mRightSurface;
			mDevice->StretchRect(srcSurface, nullptr, backBuffer, nullptr, D3DTEXF_NONE);
			mDevice->EndScene();
			mDevice->Present(nullptr, nullptr, nullptr, nullptr); //Blocks until new vsync.
			backBuffer->Release();
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CStereoDirect3D::Blt", "Exception in CStereoDirect3D Blt");
	}
	return TRUE;
}