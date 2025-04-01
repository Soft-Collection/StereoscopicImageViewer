#include "stdafx.h"
#include "CAutoMemoryDirect2D.h"
#include "../VideoCommon/CExceptionReport.h"

CAutoMemoryDirect2D::CAutoMemoryDirect2D() :
    m_pD2DFactory(NULL),
    m_pDCRT(NULL),
	m_pDWriteFactory(NULL),
	//----------------------
	m_HWnd(NULL),
	m_MemoryBitmap(NULL),
	m_ImageWidth(0),
	m_ImageHeight(0)
{
	try
	{
	    HRESULT hr;
	    hr = CreateDeviceIndependentResources();
		// Draw the pie chart with Direct2D.
		// Create the DC render target.
		hr = CreateDeviceResources();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect2D::CAutoMemoryDirect2D", "Exception in CAutoMemoryDirect2D Constructor");	
	}
}
CAutoMemoryDirect2D::~CAutoMemoryDirect2D()
{
	try
	{
		SafeRelease(&m_MemoryBitmap);
		SafeRelease(&m_pDWriteFactory);
	    SafeRelease(&m_pDCRT);
	    SafeRelease(&m_pD2DFactory);
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect2D::~CAutoMemoryDirect2D", "Exception in CAutoMemoryDirect2D Destructor");		
	}
}
/******************************************************************
*  DemoApp::CreateDeviceIndependentResources                      *
*  This method is used to create resources which are not bound    *
*  to any device. Their lifetime effectively extends for the      *
*  duration of the app.                                           *
******************************************************************/
HRESULT CAutoMemoryDirect2D::CreateDeviceIndependentResources()
{
    HRESULT hr = S_OK;
	try
	{
		// Create D2D factory
		return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect2D::CreateDeviceIndependentResources", "Exception in CAutoMemoryDirect2D CreateDeviceIndependentResources");	
	}
    return hr;
}
/******************************************************************
*  DemoApp::CreateDeviceResources                                 *
*  This method creates resources which are bound to a particular  *
*  D3D device. It's all centralized here, in case the resources   *
*  need to be recreated in case of D3D device loss (eg. display   *
*  change, remoting, removal of video card, etc).                 *
******************************************************************/
HRESULT CAutoMemoryDirect2D::CreateDeviceResources()
{
    HRESULT hr = S_OK;
	try
	{
	   if (!m_pDCRT)
	   {
	       // Create a DC render target.
	       D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), 0, 0, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT);
	       hr = m_pD2DFactory->CreateDCRenderTarget(&props, &m_pDCRT);
	       // Create a shared DirectWrite factory.
           hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	   }
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect2D::CreateDeviceResources", "Exception in CAutoMemoryDirect2D CreateDeviceResources");		
	}
    return hr;
}
/******************************************************************
*  DemoApp::DiscardDeviceResources                                *
*  Discard device-specific resources which need to be recreated   *
*  when a D3D device is lost                                      *
******************************************************************/
void CAutoMemoryDirect2D::DiscardDeviceResources()
{
	try
	{
		SafeRelease(&m_pDCRT);
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect2D::DiscardDeviceResources", "Exception in CAutoMemoryDirect2D DiscardDeviceResources");		
	}
}
void CAutoMemoryDirect2D::ReInit(HWND hWnd, int ImageWidth, int ImageHeight)
{
	try
	{
		if ((m_HWnd != hWnd) || (m_ImageWidth != ImageWidth) || (m_ImageHeight != ImageHeight))
		{
			if (m_MemoryBitmap != NULL)
			{
				SafeRelease(&m_MemoryBitmap);
			}
			//--------------------------------------------------------
			D2D_SIZE_U size = {ImageWidth, ImageHeight};
			FLOAT dpiX, dpiY;
			if (m_pD2DFactory != NULL)
			{
				m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);
			}
			D2D1_BITMAP_PROPERTIES p = {{DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE}, dpiX, dpiY};
			D2D1_RECT_F srect = {0, 0, ImageWidth, ImageHeight};
			if (m_pDCRT != NULL)
			{
				m_pDCRT->CreateBitmap(size, NULL, ImageWidth * 4, p, &m_MemoryBitmap);
			}
			//--------------------------------------------------------
			m_HWnd = hWnd;
			m_ImageWidth = ImageWidth;
			m_ImageHeight = ImageHeight;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect2D::ReInit", "Exception in CAutoMemoryDirect2D ReInit");
	}		
}
BOOL CAutoMemoryDirect2D::DrawImageBGR32(HWND hWnd, BYTE* ImageDataPtr, int ImageWidth, int ImageHeight)
{
	BOOL retVal = FALSE;
	try
	{
		ReInit(hWnd, ImageWidth, ImageHeight);
		if (m_MemoryBitmap != NULL)
		{
			D2D1_RECT_U rect = {0, 0, ImageWidth, ImageHeight};
			m_MemoryBitmap->CopyFromMemory(&rect, ImageDataPtr, ImageWidth * 4);
			retVal = TRUE;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect2D::DrawImageBGR32", "Exception in CAutoMemoryDirect2D DrawImageBGR32");		
	}
	return (retVal);
}
BOOL CAutoMemoryDirect2D::Blt()
{
	BOOL retVal = TRUE;
	try
	{
		if (m_HWnd != NULL)
		{
			HRESULT hr;
			RECT rc;
			// Get the dimensions of the client drawing area.
			GetClientRect(m_HWnd, &rc);
			// Draw the pie chart with Direct2D.
			// Create the DC render target.
			HDC hDC = ::GetDC(m_HWnd);
			if (m_pDCRT != NULL)
			{
				// Bind the DC to the DC render target.
				hr = m_pDCRT->BindDC(hDC, &rc);
				m_pDCRT->BeginDraw();
				//-----------------------------------------------------
				D2D1_RECT_F srect = {0, 0, m_ImageWidth, m_ImageHeight};
				D2D1_RECT_F drect = {(FLOAT)rc.left, (FLOAT)rc.top, (FLOAT)rc.right, (FLOAT)rc.bottom};
				if (m_MemoryBitmap != NULL)
				{
					m_pDCRT->DrawBitmap(m_MemoryBitmap, drect, 1.0, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srect);
				}
				else retVal = FALSE;
				//-----------------------------------------------------
				hr = m_pDCRT->EndDraw();
			}
			else retVal = FALSE;
			::ReleaseDC(m_HWnd, hDC);
		}
		else retVal = FALSE;
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CDirect2D::DrawImageBGR32", "Exception in CDirect2D DrawImageBGR32");		
	}
	return (retVal);
}