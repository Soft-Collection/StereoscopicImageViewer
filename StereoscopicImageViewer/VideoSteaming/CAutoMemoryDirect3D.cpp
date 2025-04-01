#include "stdafx.h"
#include "CAutoMemoryDirect3D.h"
#include "../VideoCommon/CExceptionReport.h"

CAutoMemoryDirect3D::CAutoMemoryDirect3D()
{
	try
	{
		m_HWnd = NULL;
		m_ImageWidth = 0;
		m_ImageHeight = 0;
		g_pD3D = NULL;
		g_pd3dDevice = NULL;
		g_pTexture = NULL;
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
		if (g_pTexture != NULL) g_pTexture->Release();
		if (g_pd3dDevice != NULL) g_pd3dDevice->Release();
		if (g_pD3D != NULL) g_pD3D->Release();
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::~CAutoMemoryDirect3D", "Exception in CAutoMemoryDirect3D Destructor");
	}
}
void CAutoMemoryDirect3D::ReInit(HWND hWnd, int ImageWidth, int ImageHeight)
{
	try
	{
		if ((m_HWnd != hWnd) || (m_ImageWidth != ImageWidth) || (m_ImageHeight != ImageHeight))
		{
			if (g_pTexture != NULL) g_pTexture->Release();
			if (g_pd3dDevice != NULL) g_pd3dDevice->Release();
			if (g_pD3D != NULL) g_pD3D->Release();
			//--------------------------------------------------------
			// Create the IDirect3D9 object.
			if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) return;
			// Set up the structure for creating a device.
			D3DPRESENT_PARAMETERS d3dpp;
			ZeroMemory(&d3dpp, sizeof(d3dpp));
			d3dpp.Windowed = TRUE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
			d3dpp.BackBufferCount = 1;
			d3dpp.BackBufferWidth = ImageWidth;
			d3dpp.BackBufferHeight = ImageHeight;
			// Create the device.
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))	return;
			// Create the texture.
			// We assume a ImageWidth x ImageHeight texture with 1 mip level, 0 usage, with the ARGB format.
			if (FAILED(g_pd3dDevice->CreateTexture(ImageWidth, ImageHeight, 1, 0, D3DFMT_A8R8G8B8,	D3DPOOL_MANAGED, &g_pTexture, NULL))) return;
			//--------------------------------------------------------
			m_HWnd = hWnd;
			m_ImageWidth = ImageWidth;
			m_ImageHeight = ImageHeight;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::ReInit", "Exception in CAutoMemoryDib ReInit");
	}		
}
BOOL CAutoMemoryDirect3D::DrawImageRGB32(HWND hWnd, BYTE* ImageDataPtr, int ImageWidth, int ImageHeight)
{
	BOOL retVal = FALSE;
	try
	{
		ReInit(hWnd, ImageWidth, ImageHeight);
		// Lock the texture to copy our image bytes.
		D3DLOCKED_RECT lockedRect;
		if (FAILED(g_pTexture->LockRect(0, &lockedRect, NULL, 0))) return FALSE;
		// Copy image data into the texture.
		// Since our texture width is 2 and each pixel is 4 bytes, the pitch should be at least 8 bytes.
		memcpy(lockedRect.pBits, ImageDataPtr, ImageWidth * ImageHeight * 4);
		// Unlock the texture.
		g_pTexture->UnlockRect(0);
		retVal = TRUE;
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::DrawImageRGB32", "Exception in CAutoMemoryDirect3D DrawImageRGB32");
	}
	return (retVal);
}
BOOL CAutoMemoryDirect3D::Blt()
{
	BOOL retVal = TRUE;
	try
	{
		if (g_pd3dDevice == NULL) return FALSE;
		// Clear the backbuffer to dark blue.
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
		// Begin the scene.
		if (SUCCEEDED(g_pd3dDevice->BeginScene()))
		{
			CUSTOMVERTEX vertices[] =
			{
				//        x                     y             z    rhw    tu    tv  
				{                0.0f,                 0.0f, 0.5f, 1.0f, 0.0f, 0.0f }, // top-left
				{ (FLOAT)m_ImageWidth,                 0.0f, 0.5f, 1.0f, 1.0f, 0.0f }, // top-right
				{ (FLOAT)m_ImageWidth, (FLOAT)m_ImageHeight, 0.5f, 1.0f, 1.0f, 1.0f }, // bottom-right
				{                0.0f, (FLOAT)m_ImageHeight, 0.5f, 1.0f, 0.0f, 1.0f }  // bottom-left
			};
			// Tell DirectX which vertex format we are using.
			g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
			// Set the texture to stage 0.
			g_pd3dDevice->SetTexture(0, g_pTexture);
			// Draw the quad as a triangle fan (2 triangles).
			g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(CUSTOMVERTEX));
			// End the scene.
			g_pd3dDevice->EndScene();
		}
		// Present the backbuffer contents to the display.
		g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAutoMemoryDirect3D::Blt", "Exception in CAutoMemoryDirect3D Blt");
	}
	return (retVal);
}