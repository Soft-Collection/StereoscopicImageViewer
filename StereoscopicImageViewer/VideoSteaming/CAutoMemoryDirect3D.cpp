#include "stdafx.h"
#include "CAutoMemoryDirect3D.h"
#include "../VideoCommon/CExceptionReport.h"

CAutoMemoryDirect3D::CAutoMemoryDirect3D()
{
	try
	{
		m_ImageDataPtr = NULL;
		m_ImageWidth = 0;
		m_ImageHeight = 0;
		m_cD3d = NULL;
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::~CAutoMemoryDirect3D", "Exception in CAutoMemoryDirect3D Destructor");
	}
}
CAutoMemoryDirect3D::~CAutoMemoryDirect3D()
{
	try
	{
		if (m_cD3d != NULL)
		{
			m_cD3d->m_Endup();
			delete(m_cD3d);
			m_cD3d = NULL;
		}
		if (m_ImageDataPtr != NULL)
		{
			delete(m_ImageDataPtr);
			m_ImageDataPtr = NULL;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::~CAutoMemoryDirect3D", "Exception in CAutoMemoryDirect3D Destructor");
	}
}
void CAutoMemoryDirect3D::ReInit(int ImageWidth, int ImageHeight)
{
	try
	{
		if ((m_ImageWidth != ImageWidth) || (m_ImageHeight != ImageHeight))
		{
			if (m_ImageDataPtr != NULL)
			{
				delete(m_ImageDataPtr);
				m_ImageDataPtr = NULL;
			}
			m_ImageDataPtr = new BYTE[ImageWidth * ImageHeight * 4];
			//--------------------------------------------------------
			if (m_cD3d != NULL)
			{
				m_cD3d->m_Endup();
				delete(m_cD3d);
				m_cD3d = NULL;
			}
			m_cD3d = new CD3d(NULL);
			HRESULT hr = m_cD3d->m_Setup( 0, _D3D_SURFACE_COLORFORMAT, NULL/*_hwnd*/, ImageWidth, ImageHeight, ImageWidth, ImageHeight);
			if ((hr != NOERROR) && ( hr != 1 ) && ( hr != 2 )) m_cD3d->m_Endup();
			//--------------------------------------------------------
			m_ImageWidth = ImageWidth;
			m_ImageHeight = ImageHeight;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::ReInit", "Exception in CAutoMemoryDib ReInit");
	}		
}
HRESULT CAutoMemoryDirect3D::_D3DDrawString( LPCWSTR Message, INT _nX, INT _nY, DWORD _dwWidth, DWORD _dwHeight )
{
	try
	{
		RECT	rc3DCanvas;
	#if 1
		SetRect( &rc3DCanvas, 0, 0, _dwWidth, _dwHeight );
	#else
		SetRect( &rc3DCanvas, 0, 0, m_cD3d->m_GetCurrent3DCanvasSize().cx, m_cD3d->m_GetCurrent3DCanvasSize().cy );
	#endif
		SIZE	sizeImage = { _dwWidth, _dwHeight };
		POINT	ptStartXY = { _nX, _nY };
		if (Message == NULL) return (-1);
		CHAR tempString[256] = {0, };
		for (int i = 0; ((i < 255) && (Message[i] != 0)); i++) tempString[i] = (CHAR)Message[i];
		m_cD3d->m_DrawText( rc3DCanvas,	// Target
						   sizeImage,	// Src
						   ptStartXY,
						   tempString,
						   _D3D_FONT_ALPHA,
						   _D3D_FONT_RGB );
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::_D3DDrawString", "Exception in CAutoMemoryDirect3D _D3DDrawString");
	}
	return 0;
}
HRESULT CAutoMemoryDirect3D::_D3DDrawCaptureStream( LPBYTE _lpStream, DWORD _dwStreamWidth, DWORD _dwStreamHeight, eSTREAMFORMAT _eStreamFormat )
{
	try
	{
		if( _dwStreamWidth == 0 || _dwStreamHeight == 0 ) return -1;
		RECT	rc3DCanvas;
#if 1
		SetRect( &rc3DCanvas, 0, 0, _dwStreamWidth, _dwStreamHeight );
#else
		SetRect( &rc3DCanvas, 0, 0, m_cD3d->m_GetCurrent3DCanvasSize().cx, m_cD3d->m_GetCurrent3DCanvasSize().cy );
#endif
		SIZE sizeImage = { _dwStreamWidth, _dwStreamHeight };
		CD3d::eCOLORFORMAT eSrcCF;
		if( _eStreamFormat == YUY2 )  eSrcCF = CD3d::CF_YUY2;
		else
		if( _eStreamFormat == YV12 )  eSrcCF = CD3d::CF_YV12;
		else
		if( _eStreamFormat == RGB32 ) eSrcCF = CD3d::CF_BGRX;
		else
			return -2;
		m_cD3d->m_DrawImage( rc3DCanvas,	// Target
						    sizeImage,		// Src
							eSrcCF,			// CD3d::CF_BGRX,
						    _lpStream );
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::_D3DDrawCaptureStream", "Exception in CAutoMemoryDirect3D _D3DDrawCaptureStream");
	}
	return 0;
}
BOOL CAutoMemoryDirect3D::_D3DPrimaryPresentation()
{
	BOOL retVal = FALSE;
	try
	{
		RECT rc3DCanvas;
		SetRect( &rc3DCanvas, 0, 0, m_cD3d->m_GetCurrent3DCanvasSize().cx, m_cD3d->m_GetCurrent3DCanvasSize().cy );
		retVal = m_cD3d->m_PrimaryPresentation( NULL, &rc3DCanvas );
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::_D3DPrimaryPresentation", "Exception in CAutoMemoryDirect3D _D3DPrimaryPresentation");
	}
	return (retVal);
}
BOOL CAutoMemoryDirect3D::DrawImageRGB32(BYTE* ImageDataPtr, int ImageWidth, int ImageHeight)
{
	BOOL retVal = FALSE;
	try
	{
		ReInit(ImageWidth, ImageHeight);
		if (m_ImageDataPtr != NULL)
		{
			CopyMemory(m_ImageDataPtr, ImageDataPtr, ImageWidth * ImageHeight * 4);
			retVal = TRUE;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::DrawImageRGB32", "Exception in CAutoMemoryDirect3D DrawImageRGB32");
	}
	return (retVal);
}
BOOL CAutoMemoryDirect3D::Blt(HWND hWnd)
{
	BOOL retVal = TRUE;
	try
	{
		if (hWnd != NULL)
		{
			m_cD3d->m_SetDDClipperWindow(hWnd);
			_D3DDrawCaptureStream(m_ImageDataPtr, m_ImageWidth, m_ImageHeight, CAutoMemoryDirect3D::RGB32);
			_D3DPrimaryPresentation();
		}
		else retVal = FALSE;
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::Blt", "Exception in CAutoMemoryDirect3D Blt");
	}
	return (retVal);
}