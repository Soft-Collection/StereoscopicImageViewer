#include "stdafx.h"
#include "CAutoMemoryDib.h"
#include "../VideoCommon/CExceptionReport.h"

CAutoMemoryDib::CAutoMemoryDib()
{
	try
	{
		m_MemoryDC = NULL;
		m_MemoryBitmap = NULL;
		m_pvBits = NULL;
		m_ImageWidth = 0;
		m_ImageHeight = 0;
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDib::~CAutoMemoryDib", "Exception in CAutoMemoryDib Destructor");
	}
}
CAutoMemoryDib::~CAutoMemoryDib()
{
	try
	{
		if (m_MemoryBitmap != NULL)
		{
			DeleteObject(m_MemoryBitmap);
			m_MemoryBitmap = NULL;
		}
		if (m_MemoryDC != NULL)
		{
			DeleteDC(m_MemoryDC);
			m_MemoryDC = NULL;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDib::~CAutoMemoryDib", "Exception in CAutoMemoryDib Destructor");
	}
}
void CAutoMemoryDib::ReInit(int ImageWidth, int ImageHeight)
{
	try
	{
		if ((m_ImageWidth != ImageWidth) || (m_ImageHeight != ImageHeight))
		{
			if (m_MemoryBitmap != NULL)
			{
				DeleteObject(m_MemoryBitmap);
				m_MemoryBitmap = NULL;
			}
			ZeroMemory(&m_bi, sizeof(BITMAPINFO));
			m_bi.bmiHeader.biBitCount = 32;
			m_bi.bmiHeader.biCompression = BI_RGB;
			m_bi.bmiHeader.biPlanes = 1;
			m_bi.bmiHeader.biSize = 40;
			m_bi.bmiHeader.biWidth = ImageWidth;
			m_bi.bmiHeader.biHeight = -ImageHeight;
			m_bi.bmiHeader.biSizeImage = ImageWidth * ImageHeight * 4;
			//--------------------------------------------------------
			HDC hDC = GetDC(NULL);
			m_MemoryDC = CreateCompatibleDC(hDC);
			m_MemoryBitmap = CreateDIBSection(m_MemoryDC, &m_bi, DIB_RGB_COLORS, &m_pvBits, NULL, 0);
			SelectObject(m_MemoryDC, m_MemoryBitmap);
			ReleaseDC(NULL, hDC);
			//--------------------------------------------------------
			m_ImageWidth = ImageWidth;
			m_ImageHeight = ImageHeight;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDib::ReInit", "Exception in CAutoMemoryDib ReInit");
	}		
}
BOOL CAutoMemoryDib::DrawImageRGB32(BYTE* ImageDataPtr, int ImageWidth, int ImageHeight)
{
	BOOL retVal = FALSE;
	try
	{
		ReInit(ImageWidth, ImageHeight);
		if ((m_MemoryDC != NULL) && (m_MemoryBitmap != NULL))
		{
			CopyMemory(m_pvBits, ImageDataPtr, ImageWidth * ImageHeight * 4);
			retVal = TRUE;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDib::DrawImageRGB32", "Exception in CAutoMemoryDib DrawImageRGB32");
	}
	return (retVal);
}
BOOL CAutoMemoryDib::Blt(HWND hWnd)
{
	BOOL retVal = TRUE;
	try
	{
		if (hWnd != NULL)
		{
			HDC hDC = ::GetDC(hWnd);
			RECT rect;
			GetWindowRect(hWnd, &rect);
			SetStretchBltMode(hDC, COLORONCOLOR);
			if (StretchBlt(hDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_MemoryDC, 0, 0, m_ImageWidth, m_ImageHeight, SRCCOPY) != TRUE) retVal = FALSE;
			ReleaseDC(hWnd, hDC);
		}
		else retVal = FALSE;
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDib::Blt", "Exception in CAutoMemoryDib Blt");
	}
	return (retVal);
}