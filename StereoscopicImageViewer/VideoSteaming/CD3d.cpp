/* **********************************************************************************
	import standard function prototype
*/
#include "stdafx.h"

#ifndef _AFXDLL
#include <windows.h>
#endif

/* **********************************************************************************
	import our function prototype
*/
#ifndef _AFXDLL
#include "Misc.h"
#endif

#include "colorconversion.h"

/* **********************************************************************************
	local function prototype and definition
*/
#include "CD3d.hpp"

#ifdef _VT
#include "..\Resource.h"
	#ifndef _VMR_EXPORT
	#include "..\AxVCACtl.h"
	#endif //_VMR_EXPORT
#endif // _VT

#ifndef _SAFE_RELEASE
#define _SAFE_RELEASE(c) if(c){c->Release();c=NULL;}else{}
#endif

#define _INIT_DIRECTDRAW_STRUCT(x) (ZeroMemory(&x, sizeof(x)), x.dwSize=sizeof(x))

DDPIXELFORMAT CD3d::DDPF_RGBA16 = {
	sizeof(DDPIXELFORMAT),
	DDPF_RGB|DDPF_ALPHAPIXELS ,
	0,
	16,
	0x00000F00,0x000000F0,0x0000000F,0x0000F000,
};

DDPIXELFORMAT CD3d::DDPF_RGB32 = {
	sizeof(DDPIXELFORMAT),
	DDPF_RGB,
	0,
	32,
	0x00FF0000,0x0000FF00,0x000000FF,0x00000000,
};

#ifdef _VT
DDPIXELFORMAT CD3d::DDPF_RGBA32 = {
	sizeof( DDPIXELFORMAT ),
	DDPF_RGB|DDPF_ALPHAPIXELS,
	0,
	32,
	0x00FF0000,0x0000FF00,0x000000FF,0xFF000000,
};
#endif // _VT

DDPIXELFORMAT CD3d::DDPF_RGB24 = {
	sizeof(DDPIXELFORMAT),
	DDPF_RGB,
	0,
	24,
	0x00FF0000,0x0000FF00,0x000000FF,0x00000000,
};

DDPIXELFORMAT CD3d::DDPF_RGB565 = {
	sizeof(DDPIXELFORMAT),
	DDPF_RGB,
	0,
	16,
	0x0000F800,0x000007E0,0x0000001F,0x00000000,
};

DDPIXELFORMAT CD3d::DDPF_RGB555 = {
	sizeof(DDPIXELFORMAT),
	DDPF_RGB,
	0,
	16,
	0x00007C00,0x000003E0,0x0000001F,0x00000000,
};

DDPIXELFORMAT CD3d::DDPF_YUY2 = {
	sizeof(DDPIXELFORMAT),
	DDPF_FOURCC,
	MAKEFOURCC('Y','U','Y','2'),
	0,
	0,0,0,0
};

/* **********************************************************************************
	global variable define
*/


/* **********************************************************************************
	function implementation
*/
CD3d::CD3d(void *parent):
m_pParent(parent)
{
	m_aptDDAdapterInfo = NULL;
	m_nDDAdapterCount  = 0;
	_EnumDDAdapter();

	m_nUseDDAdapterNo = 0;

	m_pDD			 = 0;
	m_pd3dDevice	 = 0;

	m_pddsPrimary	 = 0;
	m_pD3D			 = 0;
	m_pddsBackbuffer = 0;
	m_pddClipper 	 = 0;
	m_hwndClipper	 = NULL;

	m_eD3dTextureCF	= CF_NOTDEFINED;
	m_dwTextureWidth   = 0;
	m_dwTextureHeight  = 0;

	m_iUserFontSize = 0;

	m_pddsTexture = NULL;
	ZeroMemory( m_apddsTextures,sizeof(m_apddsTextures) );

	// for Draw RECTS and LINES
	m_aptRectAndLineTLV = new D3DTLVERTEX[MAX_TLVERTEXT_RECTANDLINES];
	for( int i=0; i<MAX_TLVERTEXT_RECTANDLINES; i++ ) {
		m_aptRectAndLineTLV[i] = D3DTLVERTEX(D3DVECTOR(0,0,0.9f),0.1f,0xFFFFFFFF,0x00000000,0.0f,0.0f);
	}

	// for Draw IMAGE
	m_aptImageTLV = new D3DTLVERTEX[MAX_TLVERTEXT_IMAGE];
	m_aptImageTLV[0] = D3DTLVERTEX(D3DVECTOR(   0,1024,0.9f),0.1f,0xFFFFFFFF,0x00000000,0.0f,1.0f);
	m_aptImageTLV[1] = D3DTLVERTEX(D3DVECTOR(   0,   0,0.9f),0.1f,0xFFFFFFFF,0x00000000,0.0f,0.0f);
	m_aptImageTLV[2] = D3DTLVERTEX(D3DVECTOR(1024,1024,0.9f),0.1f,0xFFFFFFFF,0x00000000,1.0f,1.0f);
	m_aptImageTLV[3] = D3DTLVERTEX(D3DVECTOR(1024,   0,0.9f),0.1f,0xFFFFFFFF,0x00000000,1.0f,0.0f);

	// for Draw FONT
	m_aptFontTLV = new D3DTLVERTEX[MAX_TLVERTEXT_FONT];
	INT i;
	for( i=0; i<MAX_TLVERTEXT_FONT; i++ ) {
		m_aptFontTLV[i] = D3DTLVERTEX(D3DVECTOR(0,0,0.9f),1.0f,0xFFFFFFFF,0x00000000,0.0f,0.0f);
	}

	// About Font
	m_dwSavedStateBlock = 0;
	m_dwDrawTextStateBlock = 0;
	m_IsAllocStateBlock	= FALSE;
	m_pddsFontTexture = NULL;

	//
	_InitColorConversionFunctions();

	//
	InitializeCriticalSection( &m_crit );

	//
	m_dw3DCanvasWidth  = 0;
	m_dw3DCanvasHeight = 0;
	
#ifdef _VT
	m_bFontChanged = FALSE;
	memset( m_pddsBrushTextures, 0, sizeof( m_pddsBrushTextures ) );
	m_csVideoSize = CSize(_MAXWIDTH_D3DCANVAS, _MAXHEIGHT_D3DCANVAS);
	m_pSphereMesh = 0;
	m_pCylinderMesh = 0;
	m_pConeMesh = 0;
	m_pddsGroundTexture = 0;
	m_bVideoPaused = FALSE;

#endif // _VT

#ifdef _DRAWRECORDS_ALL
	m_pRecords					= new tDRAWRECORD[MAX_NUM_RECORDS];
	m_pTexts[0]					= new TCHAR[MAX_TEXT_BUFFER];
	m_pTexts[MAX_NUM_RECORDS]	= m_pTexts[0] + MAX_TEXT_BUFFER;
	m_pPolygons[0]				= new MYPOLYGON[MAX_POLYGON_BUFFER];
	m_pPolygons[MAX_NUM_RECORDS]= m_pPolygons[0] + MAX_POLYGON_BUFFER;
	m_pLinePoints[0]			= new POINT[MAX_LINEPOINTS_BUFFER];
	m_pLinePoints[MAX_NUM_RECORDS] = m_pLinePoints[0] + MAX_LINEPOINTS_BUFFER;
	_ClearAllRecords();
#endif

}

CD3d::~CD3d()
{
	m_Endup();

	DeleteCriticalSection( &m_crit );

#ifdef _DRAWRECORDS_ALL
	if ( m_pRecords )
		delete[] m_pRecords;
	if ( m_pTexts[0] )
		delete[] m_pTexts[0];
	if ( m_pPolygons[0] )
		delete[] m_pPolygons[0];
	if ( m_pLinePoints[0] )
		delete[] m_pLinePoints[0];
#endif

	delete[] m_aptRectAndLineTLV;
	delete[] m_aptImageTLV;
	delete[] m_aptFontTLV;

	if( m_aptDDAdapterInfo ) delete[] m_aptDDAdapterInfo;
}

BOOL WINAPI CD3d::_lpWrapAdapterEnumCallback( GUID*		_pGUID,
											  LPSTR		_strDesc,
											  LPSTR		_strName,
											  VOID*		_lpContext,
											  HMONITOR	_hMonitor )
{
	CD3d* pcThis = (CD3d*)_lpContext;
	return pcThis->_AdapterEnumCallback( _pGUID, _strDesc, _strName, _hMonitor );
}

BOOL CD3d::_AdapterEnumCallback( GUID*	  _pGUID,
								 LPSTR	  _strDesc,
								 LPSTR	  _strName,
								 HMONITOR _hMonitor )
{
	if( m_aptDDAdapterInfo ) {
		if( _pGUID == NULL ) {
			m_aptDDAdapterInfo[m_nDDAdapterCount].fIsAvailableGUID = FALSE;	
		}
		else {
			m_aptDDAdapterInfo[m_nDDAdapterCount].fIsAvailableGUID = TRUE;	
			m_aptDDAdapterInfo[m_nDDAdapterCount].tGUID = *_pGUID;
		}

		lstrcpy( m_aptDDAdapterInfo[m_nDDAdapterCount].szDesc, _strDesc );
		lstrcpy( m_aptDDAdapterInfo[m_nDDAdapterCount].szName, _strDesc );
		m_aptDDAdapterInfo[m_nDDAdapterCount].hMonitor = _hMonitor;

//		ErrDbg(1, "m_aptDDAdapterInfo[%d].szDesc %s \n", m_nDDAdapterCount, m_aptDDAdapterInfo[m_nDDAdapterCount].szDesc );
//		ErrDbg(1, "m_aptDDAdapterInfo[%d].szName %s \n", m_nDDAdapterCount, m_aptDDAdapterInfo[m_nDDAdapterCount].szName );
//		ErrDbg(1, "m_aptDDAdapterInfo[%d].hMonitor %X \n", m_nDDAdapterCount, m_aptDDAdapterInfo[m_nDDAdapterCount].hMonitor );
	}

	m_nDDAdapterCount += 1;
	return TRUE;
}

HRESULT CD3d::_EnumDDAdapter()
{
	HRESULT	hr;

	if( m_aptDDAdapterInfo ) delete[] m_aptDDAdapterInfo;
	m_aptDDAdapterInfo = NULL;
	m_nDDAdapterCount = 0;
    hr = ::DirectDrawEnumerateEx( _lpWrapAdapterEnumCallback,
								  this,
						          DDENUM_ATTACHEDSECONDARYDEVICES |
						          DDENUM_DETACHEDSECONDARYDEVICES |
							      DDENUM_NONDISPLAYDEVICES );
	if( FAILED(hr) ) {
		_MACRO_DDERROR( hr, "DirectDrawEnumerateEx" );
		return FALSE;
	}
	if( m_nDDAdapterCount == 0 ) return -1;

	m_aptDDAdapterInfo = new tDDADAPTERINFO[m_nDDAdapterCount];
	m_nDDAdapterCount = 0;
    hr = ::DirectDrawEnumerateEx( _lpWrapAdapterEnumCallback,
								  this,
						          DDENUM_ATTACHEDSECONDARYDEVICES |
						          DDENUM_DETACHEDSECONDARYDEVICES |
							      DDENUM_NONDISPLAYDEVICES );
	return hr;
}

INT CD3d::m_GetManyDDAdapters()
{
	return m_nDDAdapterCount;
}
	
CD3d::tDDADAPTERINFO* CD3d::m_GetDDAdapterInfo( INT _nDDAdapterNo )
{ 
	if( (0 <= _nDDAdapterNo) && (_nDDAdapterNo < m_nDDAdapterCount) ) { ; }
	else return NULL;

	return &m_aptDDAdapterInfo[_nDDAdapterNo];
}

SIZE CD3d::m_GetCurrent3DCanvasSize()
{
	SIZE sizeRet = { m_dw3DCanvasWidth, m_dw3DCanvasHeight };
	return sizeRet;
}

SIZE CD3d::m_GetCurrent3DTextureSize()
{
	SIZE sizeRet = { m_dwTextureWidth, m_dwTextureHeight };
	return sizeRet;
}

HRESULT CD3d::m_Setup( INT			_nUseDDAdapterNo,
					   eCOLORFORMAT	_eD3dTextureCF, 
					   HWND			_hwndWindow,
					   INT			_n3DCanvasWidth,
					   INT		    _n3DCanvasHeight,
					   INT			_n3DTextureWidth,
					   INT		    _n3DTextureHeight
#ifdef _VMR_EXPORT
					   ,LPDIRECTDRAWSURFACE7 pddsBack
#endif
					   )
{
	HRESULT hRet = NOERROR;
	BOOL    fRet;

	do {
		if( (0 <= _nUseDDAdapterNo) && (_nUseDDAdapterNo < m_nDDAdapterCount) ) { ; }
		else {
			hRet = -1;
			break;
		}

		if( (_eD3dTextureCF == CF_YUY2)   ||
			(_eD3dTextureCF == CF_BGR565) ||
			(_eD3dTextureCF == CF_BGR555) ||
			(_eD3dTextureCF == CF_BGRX)	     ) { ; }
		else {
			hRet = -2;
			break;
		}
#ifdef _VMR_EXPORT
		fRet = _InitDDInterface( pddsBack, _hwndWindow );
#else
		fRet = _InitDDInterface( _nUseDDAdapterNo, _hwndWindow );
#endif
		if( fRet == FALSE ) {
			hRet = -3;
			break;
		}

#ifndef _VMR_EXPORT
		fRet = _InitPrimarySurface();
		if( fRet == FALSE ) {
			hRet = -4;
			break;
		}
#endif

#ifdef _VMR_EXPORT
		fRet = m_SetBackSurface( pddsBack );
		if( FALSE == fRet )
		{
			hRet = -4;
			break;
		}
#endif
		// 0, 0  : set to screen width and height of current display mode.
		fRet = _Init3DDeviceAnd3DCanvasSurface( _n3DCanvasWidth, _n3DCanvasHeight );
		if( fRet == FALSE ) {
			hRet = -5;
			break;
		}

		// 0, 0  : default value as max width and height of texture surface
		fRet = _InitImageTextureSurface( _n3DTextureWidth, _n3DTextureHeight, _eD3dTextureCF );
		if( fRet == FALSE ) {
			/*
				CF_YUY2로 시도하였을것이고... 만약 실패하였다면?
				CF_BGR565로 다시 시도한다.
			*/
#if 1
			_eD3dTextureCF = CF_BGRX;
			fRet = _InitImageTextureSurface( _n3DTextureWidth, _n3DTextureHeight, _eD3dTextureCF );
			if( fRet == FALSE ) {
				_eD3dTextureCF = CF_BGR565;
				fRet = _InitImageTextureSurface( _n3DTextureWidth, _n3DTextureHeight, _eD3dTextureCF );
				if( fRet == FALSE ) {
					hRet = -6;
					break;
				}
				hRet = 1;
			}else
				hRet = 2;
#else
			_eD3dTextureCF = CF_BGR565;
			fRet = _InitImageTextureSurface( _n3DTextureWidth, _n3DTextureHeight, _eD3dTextureCF );
			if( fRet == FALSE ) {
				hRet = -6;
				break;
			}
			hRet = 1;
#endif
		}
		m_nUseDDAdapterNo = _nUseDDAdapterNo;
		m_eD3dTextureCF   = _eD3dTextureCF;

		m_iUserFontSize = (m_iUserFontSize == 0) ? _D3D_DEFAULT_FONT_SIZE/2 : m_iUserFontSize;
		fRet = _InitFont( "Calibri",						  
						  //rickie 2008.08.13
						  m_iUserFontSize,						  
						  //(m_dw3DCanvasWidth < 1024) ? 24 : 32,
						  D3DFONT_BOLD | 
						  D3DFONT_SHADOW_LEVEL2 | 
						  D3DFONT_ANTIALIAS | 
						  (2<<16)|(2<<24) );
		if( fRet == FALSE ) {
			//ErrDbg(1, "Error _InitFont \r\n");
			hRet = -7;
			break;
		}
#ifdef _VT
		// Intialise the texture patches for the brush effects normally provided by GDI
		fRet = _InitBrushTexture( BRT_HATCH, IDB_HATCH );
#endif // _VT


	}while(0);

	if( FAILED(hRet) ) {
		_DeinitSurface();
		_DeinitDDInterface();
	}

	return hRet;
}

#ifdef _VMR_EXPORT
BOOL CD3d::_InitDDInterface( LPDIRECTDRAWSURFACE7 lpDDSurface, HWND _hwndWindow )
{
	HRESULT	hr;
	LPDIRECTDRAW7 pDD = NULL;
	// Get DD from lpDDsurface
	lpDDSurface->GetDDInterface( (VOID**)&pDD );

	// Create D3D
	LPDIRECT3D7   pD3D = NULL;
	hr = pDD->QueryInterface( IID_IDirect3D7, (PVOID*)&pD3D );
	if( FAILED(hr) ) {
		pDD->Release();
		_MACRO_DDERROR( hr, "QueryInterface(IID_IDirect3D7)" );
		return FALSE;
	}

	hr = pDD->SetCooperativeLevel( _hwndWindow, DDSCL_NORMAL );
	if( FAILED(hr) ) {
		pDD->Release();
		_MACRO_DDERROR( hr, "SetCooperativeLevel" );
		return FALSE;
	}

	m_pDD	= pDD;
	m_pD3D	= pD3D;

	return TRUE;
}
#endif

BOOL CD3d::_InitDDInterface( INT _nUseDDAdapterNo, HWND _hwndWindow )
{
	HRESULT	hr;

	GUID*	  pGUID	   = NULL;

	pGUID = NULL;
	if( m_aptDDAdapterInfo[_nUseDDAdapterNo].fIsAvailableGUID == TRUE ) {
		pGUID = &m_aptDDAdapterInfo[_nUseDDAdapterNo].tGUID;
	}

	LPDIRECTDRAW7 pDD = NULL;
	hr = ::DirectDrawCreateEx( pGUID, (PVOID*)&pDD, IID_IDirectDraw7, NULL );
	if( FAILED(hr) ) {
		_MACRO_DDERROR( hr, "DirectDrawCreateEx" );
		return FALSE;
	}

	LPDIRECT3D7   pD3D = NULL;
	hr = pDD->QueryInterface( IID_IDirect3D7, (PVOID*)&pD3D );
	if( FAILED(hr) ) {
		pDD->Release();
		_MACRO_DDERROR( hr, "QueryInterface(IID_IDirect3D7)" );
		return FALSE;
	}

	hr = pDD->SetCooperativeLevel( _hwndWindow, DDSCL_NORMAL );
	if( FAILED(hr) ) {
		pDD->Release();
		_MACRO_DDERROR( hr, "SetCooperativeLevel" );
		return FALSE;
	}


	m_pDD	= pDD;
	m_pD3D	= pD3D;

	return TRUE;
}

VOID CD3d::_DeinitDDInterface()
{
	_SAFE_RELEASE( m_pD3D );
#ifndef _VMR_EXPORT
	_SAFE_RELEASE( m_pDD );
#endif
}

BOOL CD3d::_InitPrimarySurface()
{
	if( m_pDD == NULL ) return FALSE;

	LPDIRECTDRAW7		 pDD = m_pDD;
	LPDIRECTDRAWSURFACE7 pddsPrimary = NULL;

	pddsPrimary = _CreateCustomSurface( pDD, SURFACETYPE_PRIMARY, NULL/*n/a*/, 0/*n/a*/, 0/*n/a*/ );
	if( pddsPrimary == NULL ) return FALSE;

	HRESULT hr = pDD->CreateClipper( 0, &m_pddClipper, NULL );
	if( FAILED(hr) ) {
		_MACRO_DDERROR( hr, "CreateClipper" );
		return FALSE;
	}

	hr = pddsPrimary->SetClipper( m_pddClipper );
	if( FAILED(hr) ) {
		_MACRO_DDERROR( hr, "SetClipper" );
		return FALSE;
	}

	m_pddsPrimary = pddsPrimary;
	return TRUE;
}

#ifdef _VMR_EXPORT
BOOL CD3d::m_SetBackSurface( LPDIRECTDRAWSURFACE7 pddsBack )
{
	if( pddsBack == NULL ) return FALSE;

	m_pddsBackbuffer = pddsBack;

	LPDIRECT3D7		pD3D;
	LPDIRECTDRAW7	pDD = m_pDD;
	HRESULT hr;
	hr = pDD->QueryInterface( IID_IDirect3D7, (PVOID*)&pD3D );
	if( FAILED(hr) ) {
		_MACRO_DDERROR( hr, "IID_IDirect3D7 QueryInterface" );
		return FALSE;
	}

	hr = pD3D->CreateDevice( IID_IDirect3DHALDevice, m_pddsBackbuffer, (LPDIRECT3DDEVICE7*)&m_pd3dDevice );
	if( FAILED(hr) ) {
		_MACRO_DDERROR( hr, "CreateDevice IID_IDirect3DHALDevice" );
		return FALSE;
	}
	_SAFE_RELEASE( pD3D );

	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0, 1.0, 0 );

	return TRUE;
}
#endif

#ifndef _VMR_EXPORT
BOOL CD3d::_Init3DDeviceAnd3DCanvasSurface( INT _nWidth, INT _nHeight )
{
	if( m_pDD == NULL ) return FALSE;

	HRESULT	hr;

	LPDIRECTDRAW7		 pDD			 = m_pDD;
	LPDIRECT3DDEVICE7    pd3dDevice      = NULL;
	LPDIRECTDRAWSURFACE7 pddsBackBuffer  = NULL;

	DDSURFACEDESC2	ddsdtemp;
	_INIT_DIRECTDRAW_STRUCT( ddsdtemp );

	if( 0 == _nWidth || 0 == _nHeight ) {
		// 현재 스크린 해상도 만큼 잡아 버린다.
		// 사실 Canvas는 해상도 보다 클 이유가 없다.
		pDD->GetDisplayMode( &ddsdtemp );
		_nWidth  = ddsdtemp.dwWidth;
		_nHeight = ddsdtemp.dwHeight;
	}

	// 3DDevice에 붙일 Backbuffer surface(3D Canvas)를 만든다. - TextureSurface와는 다르다. 
	pddsBackBuffer = _CreateCustomSurface( pDD, 
										   SURFACETYPE_3DCANVAS, 
										   &pd3dDevice,
										   _nWidth,
										   _nHeight );
	BOOL	fRet;
	if( (pd3dDevice != NULL) && (pddsBackBuffer != NULL) )
	{
		m_pd3dDevice	 = pd3dDevice;
		m_pddsBackbuffer = pddsBackBuffer;

		m_dw3DCanvasWidth  = _nWidth;
		m_dw3DCanvasHeight = _nHeight;

		hr = pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
		fRet = TRUE;

#ifdef _VT
		InitGridMesh();
#endif //_VT
	}
	else
	{
		if( pddsBackBuffer ) {
			LPDIRECTDRAWSURFACE7 lpAttachedSurf = NULL;
			pddsBackBuffer->EnumAttachedSurfaces( &lpAttachedSurf, _lpEnumSurfacesCallback2 );
			pddsBackBuffer->DeleteAttachedSurface( 0, lpAttachedSurf );
			_SAFE_RELEASE( lpAttachedSurf );
		}
		_SAFE_RELEASE( pddsBackBuffer );
		_SAFE_RELEASE( pd3dDevice );
		fRet = FALSE;
	}

	return fRet;
}

#else
BOOL CD3d::_Init3DDeviceAnd3DCanvasSurface( INT _nWidth, INT _nHeight )
{
	if( m_pDD == NULL ) return FALSE;

	HRESULT	hr;

	LPDIRECTDRAW7		 pDD			 = m_pDD;
	LPDIRECT3DDEVICE7    pd3dDevice      = m_pd3dDevice;
	LPDIRECTDRAWSURFACE7 pddsBackBuffer  = m_pddsBackbuffer;

	DDSURFACEDESC2	ddsdtemp;
	_INIT_DIRECTDRAW_STRUCT( ddsdtemp );

	if( 0 == _nWidth || 0 == _nHeight ) {
		// 현재 스크린 해상도 만큼 잡아 버린다.
		// 사실 Canvas는 해상도 보다 클 이유가 없다.
		pDD->GetDisplayMode( &ddsdtemp );
		_nWidth  = ddsdtemp.dwWidth;
		_nHeight = ddsdtemp.dwHeight;
	}

	BOOL	fRet = TRUE;
	if( (pd3dDevice != NULL) && (pddsBackBuffer != NULL) )
	{
		m_pd3dDevice	 = pd3dDevice;
		m_pddsBackbuffer = pddsBackBuffer;

		m_dw3DCanvasWidth  = _nWidth;
		m_dw3DCanvasHeight = _nHeight;

		hr = pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
		fRet = TRUE;
	}

#ifdef _VT
		InitGridMesh();
#endif //_VT


	return fRet;
}
#endif

BOOL CD3d::_InitImageTextureSurface( INT _nWidth, INT _nHeight, eCOLORFORMAT _eD3dTextureCF )
{
	LPDIRECTDRAW7 pDD  = m_pDD;
	LPDIRECTDRAWSURFACE7 apddsTextures[8] = {0,0,0,0,0,0,0,0};

	DDPIXELFORMAT tddPF;
	if(_eD3dTextureCF == CF_YUY2)   tddPF = DDPF_YUY2;
	if(_eD3dTextureCF == CF_BGR565) tddPF = DDPF_RGB565;
	if(_eD3dTextureCF == CF_BGR555) tddPF = DDPF_RGB555;
	if(_eD3dTextureCF == CF_BGRX)   tddPF = DDPF_RGB32;

#if 0
	// Texture를 Backbuffer로 옮길때 성능향상을 하기위해 최소한의 적당한 크기를 사용하기위해
	// 여러 크기로 미리 만들어 두지만 메모리낭비가 심하여 여러 채널을 동시에 다루어야 하는경우
	// 부작용이 생긴다. 하지만 성능은 아무래도 딱 맞게 하는것이 아주 좋다. 크기가 차이가 날수록 심해진다.
	apddsTextures[0] = _CreateCustomSurface( pDD, SURFACETYPE_AGPTEXTURE, &tddPF, 256, 128 );
	apddsTextures[1] = _CreateCustomSurface( pDD, SURFACETYPE_AGPTEXTURE, &tddPF, 256, 256 );
	apddsTextures[2] = _CreateCustomSurface( pDD, SURFACETYPE_AGPTEXTURE, &tddPF, 512, 128 );
	apddsTextures[3] = _CreateCustomSurface( pDD, SURFACETYPE_AGPTEXTURE, &tddPF, 512, 256 );
	apddsTextures[4] = _CreateCustomSurface( pDD, SURFACETYPE_AGPTEXTURE, &tddPF, 512, 512 );
	apddsTextures[5] = _CreateCustomSurface( pDD, SURFACETYPE_AGPTEXTURE, &tddPF,1024, 256 );
	apddsTextures[6] = _CreateCustomSurface( pDD, SURFACETYPE_AGPTEXTURE, &tddPF,1024, 512 );
	apddsTextures[7] = _CreateCustomSurface( pDD, SURFACETYPE_AGPTEXTURE, &tddPF,1024,1024 );
#else
	// 가장 큰 것 1024 x 1024만 연다.
	if( apddsTextures[7] == NULL ) {
		/*
			VIA Chip 마더보드 build-in VGA인 경우 Texture의 크기가 제한적이다. 즉 아무 크기나 되지 않는다.
			아래와 같이 1024로 연다.
		*/
		/*
			SIDE-EFFECT : NOTEBOOK같은 경우 1024만큼 열수가 없을텐데.... 아직 모름;;
		*/

	    // 참고 : _FindSuitableTextureSurface 함수에서 텍스쳐 크기를...
		apddsTextures[7] = _CreateCustomSurface( pDD, SURFACETYPE_AGPTEXTURE, &tddPF, _nWidth, _nHeight );
		if( NULL == apddsTextures[7] ) {
			/*
				NOTEBOOK같은 것은 AGP를 지원하지 않을수도 있다. 이런 경우 VideoMemory에 만든다.
			*/
			apddsTextures[7] = _CreateCustomSurface( pDD, SURFACETYPE_VIDMEMTEXTURE, &tddPF, _nWidth, _nHeight );
			if( NULL == apddsTextures[7] ) {
				/*
					이것도 안 만들어 지면 어떡하나???
					텍스쳐 크기를 줄여 봐야 하나???
				*/
				//ErrDbg(1, "FAIL! _CreateCustomSurface VIDEOMEM apddsTextures[7] has to Align 16bytes bound?? %Xh \r\n", apddsTextures[7]);
			}
			else {
				//ErrDbg(1, "OK! _CreateCustomSurface VIDEOMEM apddsTextures[7] has to Align 16bytes bound %Xh \r\n", apddsTextures[7]);
			}
		}
		else {
			//ErrDbg(1, "OK! _CreateCustomSurface AGPTX apddsTextures[7] has to Align 16bytes bound %Xh \r\n", apddsTextures[7]);
		}

	}
#endif

	BOOL	fRet;
	if( apddsTextures[7] ) {
		CopyMemory( m_apddsTextures, apddsTextures, sizeof(m_apddsTextures) );
		fRet = TRUE;

		m_dwTextureWidth  = _nWidth;
		m_dwTextureHeight = _nHeight; 
	}
	else {
		// 안 만들어진게 있으면 어떡하지? 메모리가 부족한가?
		for( int i=0; i<8; i++ ) {
			_SAFE_RELEASE( apddsTextures[i] );
		}
		fRet = FALSE;
	}

	return fRet;
}

VOID CD3d::_DeinitSurface()
{
	for( int i=0; i<8; i++ ) {
		_SAFE_RELEASE( m_apddsTextures[i] );
	}

	// For VCAMetaRender.dll, we do not need to release BackBuffer
#ifndef _VMR_EXPORT
	if( m_pddsBackbuffer ) {
		HRESULT hRetLost = m_pddsBackbuffer->IsLost();
		if( hRetLost == DD_OK )
		{
			LPDIRECTDRAWSURFACE7 lpAttachedSurf;
			m_pddsBackbuffer->EnumAttachedSurfaces( &lpAttachedSurf, _lpEnumSurfacesCallback2 );
			m_pddsBackbuffer->DeleteAttachedSurface( 0, lpAttachedSurf );
			_SAFE_RELEASE( lpAttachedSurf );
		}
	}
	_SAFE_RELEASE( m_pddsBackbuffer );
#endif
	_SAFE_RELEASE( m_pd3dDevice );

#ifndef _VMR_EXPORT
	// For VCAMetaRender.dll, we do not need to release Primary
	if( m_pddsPrimary ) {
		m_pddsPrimary->SetClipper( NULL );
	}
	_SAFE_RELEASE( m_pddsPrimary );
#endif
	_SAFE_RELEASE( m_pddClipper );

#ifdef _VT
		DeInitGridMesh();
#endif //_VT

}

DWORD CD3d::_GetFontAreaHeight( HFONT _hFont, DWORD _dwWidth, int _nMargin_x, int _nMargin_y )
{
	HDC   hDC = GetDC(NULL);
	HFONT hOldfont = (HFONT)SelectObject( hDC, _hFont );

	char str[4];
	int  x=0, y=0;
	SIZE size = {0,};

	// ASCII from 'space'(32) to ''(127)
	for( char c = 32; c < 127; c++ ) {
		str[0] = c;
		::GetTextExtentPoint32( hDC, str, 1, &size );

		if( (DWORD)( x + size.cx + 1 + 2 ) > _dwWidth ) {
			x = 0;
			y += size.cy + 3 + _nMargin_y;
		}
		x += size.cx + 1 + _nMargin_x;
	}

	SelectObject( hDC, hOldfont );
	ReleaseDC( NULL, hDC );
	
	return ( y + size.cy + 1 );
}

BOOL CD3d::_InitFont( LPCSTR _lpszFontName, DWORD dwHeight, DWORD dwFlags )
{
	INT	  offset_x    = (char)( (dwFlags>>16) & 0xFF );
	INT	  offset_y    = (char)( (dwFlags>>24) & 0xFF );
	DWORD dwBold      = ( dwFlags & D3DFONT_BOLD )	    ? FW_BOLD  : FW_NORMAL;
	DWORD dwItalic    = ( dwFlags & D3DFONT_ITALIC )    ? TRUE	   : FALSE;
	DWORD dwAntiAlias = ( dwFlags & D3DFONT_ANTIALIAS ) ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY;

	int shadowlevel = 0;								  // no shadow--> shadow offset is ignored.
	if( dwFlags & D3DFONT_SHADOW_LEVEL1 ) shadowlevel = 1;// blur 1 time.
	else
	if( dwFlags & D3DFONT_SHADOW_LEVEL2 ) shadowlevel = 2;// blur 2 times.
	else
	if( dwFlags & D3DFONT_SHADOW_LEVEL3 ) shadowlevel = 3;// blur 3 times.

	int margin_x = shadowlevel * 2;
	int margin_y = shadowlevel * 2;
	if( shadowlevel < abs(offset_x) ) margin_x += abs( shadowlevel - abs(offset_x) );
	if( shadowlevel < abs(offset_y) ) margin_y += abs( shadowlevel - abs(offset_y) );

	m_def_hint = margin_x + 1;

	m_dwFontWidth = 256;
	if( dwHeight >= 50 ) m_dwFontWidth = 512;

	HFONT hFont = ::CreateFont( dwHeight, 0, 0, 0, dwBold, dwItalic,
							    FALSE,
							    FALSE, 
							    DEFAULT_CHARSET, 
							    OUT_DEFAULT_PRECIS,
							    CLIP_DEFAULT_PRECIS,
							    dwAntiAlias,
							    VARIABLE_PITCH,
							    _lpszFontName );

	DWORD height = _GetFontAreaHeight( hFont, m_dwFontWidth, margin_x, margin_y );
	if( height <=   64 ) m_dwFontHeight = 64;
	else
	if( height <=  128 ) m_dwFontHeight = 128;
	else
	if( height <=  256 ) m_dwFontHeight = 256;
	else
	if( height <=  512 ) m_dwFontHeight = 512;
	else
	if( height <= 1024 ) m_dwFontHeight = 1024;
	
	m_pddsFontTexture = _CreateCustomSurface( m_pDD, SURFACETYPE_MANAGEDTEXTURE, &DDPF_RGBA16, m_dwFontWidth, m_dwFontHeight );
	if( m_pddsFontTexture == NULL ) {
		DeleteObject( hFont );
		return FALSE;
	}

	// 모든 ASCII 글자를 한판의 Bitmap으로 미리 만들어 둔다.
	BITMAPINFO	bmi = {0,};
	bmi.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth		=  (int)m_dwFontWidth;
	bmi.bmiHeader.biHeight		= -(int)m_dwFontHeight;
	bmi.bmiHeader.biPlanes		= 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount	= 32;

	// Create a DC and a bitmap for the font
	DWORD*	pdwBmBits;
	HDC 	hDC 	  = CreateCompatibleDC( NULL );
	HBITMAP hbmBitmap = CreateDIBSection( hDC, 
										  &bmi,
										  DIB_RGB_COLORS,
										  (VOID**)&pdwBmBits,
										  NULL,
										  0 );
	SetMapMode(   hDC, MM_TEXT );
	SelectObject( hDC, hbmBitmap );
	SelectObject( hDC, hFont );
	SetTextColor( hDC, RGB(255,255,255) );
	SetBkColor(   hDC, 0x00000000 );
	SetTextAlign( hDC, TA_TOP );

	//먼저 검정색으로 클리어한다.
	ZeroMemory( pdwBmBits, m_dwFontWidth * m_dwFontHeight * 4 );	// 32bit RGB

	DWORD x = 0;
	DWORD y = 0;
	SIZE size = {0,};
	for( char c = 32; c < 127; c++ )
	{
		::GetTextExtentPoint32( hDC, &c, 1, &size );
		if( (DWORD)( x + size.cx + 1 + margin_x ) > m_dwFontWidth ) {
			x = 0;
			y += size.cy + 3 + margin_y;
		}

		::ExtTextOut( hDC,
				      x + 1 + shadowlevel,
					  y + 1 + shadowlevel,
					  ETO_OPAQUE,
					  NULL,
					  &c, 1, NULL );

		m_fTexCoords[c-32][0] = ((float)(x+0))/m_dwFontWidth;
		m_fTexCoords[c-32][1] = ((float)(y+0))/m_dwFontHeight;
		m_fTexCoords[c-32][2] = ((float)(x+1+size.cx+margin_x))/m_dwFontWidth;
		m_fTexCoords[c-32][3] = ((float)(y+1+size.cy+margin_y))/m_dwFontHeight;
		
		x += size.cx + 1 + margin_x;
	}//for( char c = 32; c < 127; c++ )

	//
	BYTE* blured = new BYTE[ m_dwFontWidth * m_dwFontHeight ];
	for( DWORD i=0; i < m_dwFontWidth * m_dwFontHeight; i++ ) {
		blured[i] = (BYTE)( pdwBmBits[i] & 0xFF );	// AlphaChannel만 긁어온다.
	}

	for( int sl = 0; sl < shadowlevel; sl++ )
	{
		for( y=0; y < m_dwFontHeight; y++ )
		{
			int y1 = y-1;
			int y2 = y+1;

			y1 = max( 0, y1 );
			y2 = min( (INT)m_dwFontHeight-1, y2 );

			for( x=0; x < m_dwFontWidth; x++ )
			{
				int x1 = x - 1,
					x2 = x + 1;

				x1 = max( 0, x1 );
				x2 = min( (INT)m_dwFontWidth - 1, x2 );

				DWORD w = m_dwFontWidth;
				blured[y * w + x] =
				((blured[y1*w+x1]&0xFF) + (blured[y1*w+x]&0xFF) + (blured[y1*w+x2]&0xFF) +
				 (blured[ y*w+x1]&0xFF) + (blured[ y*w+x]&0xFF) + (blured[ y*w+x2]&0xFF) +
				 (blured[y2*w+x1]&0xFF) + (blured[y2*w+x]&0xFF) + (blured[y2*w+x2]&0xFF)+4) / 9;
			}
		}//for( y=0; y < m_dwFontHeight; y++ )
	}//for( int sl = 0; sl < shadowlevel; sl++ )

	// Lock the surface and write the alpha values for the set pixels
	WORD* pDst16;
	DWORD dwPitch;
	if( FALSE == _GetSurfacePointer( m_pddsFontTexture, NULL, (BYTE**)&pDst16, &dwPitch ) ) {
		delete[] blured;
		DeleteObject( hbmBitmap );
		DeleteDC( hDC );
		DeleteObject( hFont );
		return FALSE;
	}

	for( y = 0; y < m_dwFontHeight; y++ )
	{
		int y1 = y - offset_y;
		y1 = max( 0, y1 );
		y1 = min( (INT)m_dwFontHeight-1, y1 );

		for( x = 0; x < m_dwFontWidth; x++ )
		{
			int x1 = x - offset_x;
			x1 = max( 0, x1 );
			x1 = min( (INT)m_dwFontWidth-1, x1 );

			DWORD w = m_dwFontWidth;
			int bAlpha = ( (int)blured[ w * y1 + x1 ] + 7 ) >> 4;
			bAlpha = min( 15, bAlpha );

			int b = (BYTE)( pdwBmBits[ w * y + x ] & 0xFF );
			int c = (b) >> 4;
			c = c | ( c << 4 ) | ( c << 8 );

			int bAlpha2 = (b) >> 4;
			bAlpha = max( bAlpha, bAlpha2 );

			*pDst16++ = (WORD)( (bAlpha << 12) | c );
		}
	}
	_ReleaseSurfacePointer( m_pddsFontTexture );

	//
	delete[] blured;
	DeleteObject( hbmBitmap );
	DeleteDC( hDC );
	DeleteObject( hFont );

	if(!_MakeStateBlock( m_pddsFontTexture ))
	{
		return FALSE;
	}
	m_IsAllocStateBlock = TRUE;

	return TRUE;
}

BOOL CD3d::_DeinitFont()
{
	// Delete the state blocks
	if( m_IsAllocStateBlock ) 
	{
		if( m_pd3dDevice )
		{
			if( m_dwSavedStateBlock ) {
				m_pd3dDevice->DeleteStateBlock( m_dwSavedStateBlock );
			}
			if( m_dwDrawTextStateBlock ) {
				m_pd3dDevice->DeleteStateBlock( m_dwDrawTextStateBlock );
			}
		}
		m_dwSavedStateBlock    = 0L;
		m_dwDrawTextStateBlock = 0L;
		m_IsAllocStateBlock = FALSE;
	}

	_SAFE_RELEASE( m_pddsFontTexture );

	return TRUE;
}

#ifdef _VT

BOOL CD3d::_InitBrushTexture( eBRUSHTYPE brushType, UINT bitmapId )
{
//	ErrDbgX( 1, "_InitBrushTexture, m_pddsBrushTextures[%d]=%d\n",  brushType, m_pddsBrushTextures[brushType] );

	// Initialise fill pattern textures
	if ( m_pddsBrushTextures[brushType] != 0 )
	{
		_DeinitBrushTexture( brushType );
	}
	ASSERT( m_pddsBrushTextures[brushType] == 0 );

	unsigned char bits[ 16 * 16 * 3 + 4];

	m_pddsBrushTextures[brushType] = _CreateCustomSurface( m_pDD, SURFACETYPE_MANAGEDTEXTURE, &DDPF_RGBA32, 16, 16 );
	if( !m_pddsBrushTextures[brushType] )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	// Load the hatch bitmap
#ifdef _VMR_EXPORT
	HBITMAP hBmp = LoadBitmap( GetModuleHandle( _T("VCAMetaRender.dll"))/*theApp.hInst*/, MAKEINTRESOURCE( bitmapId ) );
#else
#ifdef _DEBUG
	HBITMAP hBmp = LoadBitmap( GetModuleHandle( _T( "AxVCA_d.ocx"))/*theApp.hInst*/, MAKEINTRESOURCE( bitmapId ) );
#else
	HBITMAP hBmp = LoadBitmap( GetModuleHandle( _T( "AxVCA.ocx"))/*theApp.hInst*/, MAKEINTRESOURCE( bitmapId ) );
#endif
#endif
	ASSERT( hBmp );

	BITMAPINFO bmi;
	memset( &bmi, 0, sizeof( BITMAPINFO ) );
	bmi.bmiHeader.biSize		= sizeof( BITMAPINFOHEADER );
	bmi.bmiHeader.biWidth		= 16;
	bmi.bmiHeader.biHeight		= 16;
	bmi.bmiHeader.biPlanes		= 1;
	bmi.bmiHeader.biCompression	= BI_RGB;
	bmi.bmiHeader.biBitCount	= 24;

	HDC hDC = ::GetDC( AfxGetMainWnd()->GetSafeHwnd() );
	::GetDIBits( hDC, hBmp, 0, 16, bits, &bmi, DIB_RGB_COLORS );
	::ReleaseDC( NULL, hDC );

	DeleteObject(hBmp);

	// Lock the surface and transfer the bits...
	unsigned char *pBytes;
	unsigned char ucAlpha = 0xF0;
	DWORD dwPitch;
	if( _GetSurfacePointer( m_pddsBrushTextures[brushType], NULL, &pBytes, &dwPitch ) )
	{
		unsigned char *pLine = pBytes;
		for( int y = 0; y < 16; y++, pLine += dwPitch )
		{
			unsigned char *pByte = pLine;
			for( int x = 0; x < 16; x++ )
			{
				int offset = ((y*16) + x) * 3;
				*pByte++ = bits[ offset + 0 ];
				*pByte++ = bits[ offset + 1 ];
				*pByte++ = bits[ offset + 2 ];
				*pByte++ = bits[offset] == 0x00 ? 0x00 : 0xF0;
			}
		}

		_ReleaseSurfacePointer( m_pddsBrushTextures[brushType] );
		
	}
	else
	{
		ASSERT( FALSE );
	}

	DeleteObject(hBmp);

	return TRUE;



}

BOOL CD3d::_DeinitBrushTexture( eBRUSHTYPE brushType )
{
	_SAFE_RELEASE( m_pddsBrushTextures[brushType] );

	m_pddsBrushTextures[brushType] = 0;

	return FALSE;
}

#endif // _VT

BOOL CD3d::m_GetTextExtent( LPCSTR _lpszText, SIZE* _psizePixel
#ifdef _VT
							,float	fScale
#endif // _VT)
		)
{
	if( _lpszText == NULL ) return FALSE;
	if( _psizePixel == NULL ) return FALSE;
	
	float fRowWidth  = 0.0f;
	float fRowHeight = (m_fTexCoords[0][3] - m_fTexCoords[0][1]) * m_dwFontHeight;
	float fWidth	 = 0.0f;
	float fHeight	 = fRowHeight;
	int	  Space		 = 0;

#ifdef _VT
	if( fScale < 1.0f )
	{
		// Automatically adjust spacing
		Space = (int)(fScale + 0.5f);
	}
#endif // _VT


	while( *_lpszText )
	{
		char c = *_lpszText++;
		
		if( c == '\n' )
		{
			fRowWidth = 0.0f;
			fHeight  += fRowHeight;
		}
		if( c < ' ' ) continue;
		
		float tx1 = m_fTexCoords[c-32][0];
		float tx2 = m_fTexCoords[c-32][2];

#ifdef _VT
		fRowWidth += fScale*( tx2 - tx1 ) * m_dwFontWidth - m_def_hint + Space;
#else
		fRowWidth += (float)1.0*( tx2 - tx1 ) * m_dwFontWidth - m_def_hint + Space;
#endif
		fWidth = max( fRowWidth, fWidth );
	}
	
	_psizePixel->cx = (int)fWidth;
	_psizePixel->cy = (int)fHeight;
	
	return TRUE;
}

D3DTLVERTEX CD3d::_InitFontVertex( const D3DXVECTOR4& p, D3DCOLOR color, float tu, float tv )
{
	D3DTLVERTEX v;
	v.dvSX    = p.x;
	v.dvSY    = p.y;
	v.dvSZ    = p.z;
	v.dvRHW   = p.w;

	v.dcColor = color;
	v.dcSpecular = 0;
	v.dvTU    = tu;
	v.dvTV    = tv;

	return v;
}

BOOL CD3d::_DrawText( float			sx,
					  float			sy,
					  RECT			rc,
					  DWORD			dwColor,
					  const char*	strText,
#ifdef _VT
					  float			fScale,
#endif // _VT
					  int			Space )
{
	if( m_pd3dDevice == NULL ) return FALSE;

	// Setup renderstate
	m_pd3dDevice->CaptureStateBlock( m_dwSavedStateBlock );
	m_pd3dDevice->ApplyStateBlock( m_dwDrawTextStateBlock );

	// Fill vertex buffer
	DWORD		  dwNumVtx = 0;
	D3DTLVERTEX*  pVx = m_aptFontTLV;
	float		  sxOrg = sx;

#ifdef _VT
	if( fScale < 1.0f )
	{
		// Automatically adjust spacing
		Space = (int)(fScale + 0.5f);
	}
#endif // _VT

	while( *strText )
	{
		char c = *strText++;
		if( c == '\n' ) {
			sy += ( m_fTexCoords['A'-32][3] - m_fTexCoords['A'-32][1] ) * m_dwFontHeight;
			sx = sxOrg;
			continue;
		}
		if( c < ' ' ) continue;

		float *TexCoords = m_fTexCoords[c-32];
		float tx1 = TexCoords[0];
		float ty1 = TexCoords[1];
		float tx2 = TexCoords[2];//+2.0f/m_dwFontWidth;
		float ty2 = TexCoords[3];//+2.0f/m_dwFontHeight;
		
		float w = (tx2 - tx1) * m_dwFontWidth;
		float h = (ty2 - ty1) * m_dwFontHeight;

//		w -= 8.5f;
//      h -= 8.5f;

#ifdef _VT
		// Apply scaling if specified
		w *= fScale;
		h *= fScale;


#endif // _VT

		//clip check...
#ifndef _VT
		// Do not clip it for VT, sometimes it won't draw the counter name when the counter is too close to the edge
		if( sx     >= rc.left  &&
			sy     >= rc.top   &&
			sx + w <= rc.right &&
			sy + h <= rc.bottom   )
#endif
		{
			*pVx++ = _InitFontVertex( D3DXVECTOR4(sx+0-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx1, ty2 );
			*pVx++ = _InitFontVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx1, ty1 );
			*pVx++ = _InitFontVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx2, ty2 );
			*pVx++ = _InitFontVertex( D3DXVECTOR4(sx+w-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx2, ty1 );
			*pVx++ = _InitFontVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx2, ty2 );
			*pVx++ = _InitFontVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx1, ty1 );
			dwNumVtx += 6;

			// MAX_TLVERTEXT_FONT 보다 더 많은 글자를 뿌려야 된다면 일단 여기서 한번 뿌려서 털어낸다.
			if( dwNumVtx > (MAX_TLVERTEXT_FONT - 6) ) {
				m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, m_aptFontTLV, dwNumVtx, 0 );
				pVx = m_aptFontTLV;
				dwNumVtx = 0L;
			}
		}
		
		sx += ( w - m_def_hint + Space );
	}//while( *strText )
	
	// 위에서 털던 그렇지 않던 MAX_TLVERTEXT_FONT 만큼 안되는 것들을 여기서 그린다.
	if( dwNumVtx > 0 ) {
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, m_aptFontTLV, dwNumVtx, 0 );
	}

	// Restore the modified renderstates
	m_pd3dDevice->ApplyStateBlock( m_dwSavedStateBlock );

	return TRUE;
}

HRESULT WINAPI CD3d::_lpEnumSurfacesCallback2( LPDIRECTDRAWSURFACE7	lpDDSurface,
											   LPDDSURFACEDESC2		lpDDSurfaceDesc,
											   LPVOID				lpContext )
{
	LPDIRECTDRAWSURFACE7* lppSurf = (LPDIRECTDRAWSURFACE7* )lpContext;
	*lppSurf = lpDDSurface;

	return DDENUMRET_CANCEL;
}

#ifdef _VT
HRESULT WINAPI CD3d::_lpEnumSurfacesCallback3( LPDIRECTDRAWSURFACE7	lpDDSurface,
											   LPDDSURFACEDESC2		lpDDSurfaceDesc,
											   LPVOID				lpContext )
{
	HRESULT hr = lpDDSurface->Restore();
	lpDDSurface->EnumAttachedSurfaces( NULL, _lpEnumSurfacesCallback3 );

	return DDENUMRET_OK;
}
#endif

BOOL CD3d::m_Endup()
{
	_DeinitFont();
	_DeinitSurface();
	_DeinitDDInterface();

#ifdef _VT
	_DeinitBrushTexture( BRT_HATCH );
#endif // _VT

	return TRUE;
}

HWND CD3d::m_GetDDClipperWindow()
{
	return m_hwndClipper;
}

BOOL CD3d::m_SetDDClipperWindow( HWND _hwnd )
{
	if( m_pddClipper == NULL ) return FALSE;

	HRESULT hr = m_pddClipper->SetHWnd( 0, _hwnd );
	if( FAILED(hr) ) {
		_MACRO_DDERROR( hr, "Clipper->SetHWnd" );
	}
	else {
		m_hwndClipper = _hwnd;
	}

	return (hr == NOERROR) ? TRUE : FALSE;
}

BOOL CD3d::m_DrawImage( RECT		 _rc3DCanvas,
						SIZE		 _sizeSrcImage,
						eCOLORFORMAT _eCFSrcImage,
						BYTE* const  _pImage )
{
	tDRAWRECORD	atDrawRecords[1];
	ZeroMemory( atDrawRecords, sizeof(atDrawRecords) );

	atDrawRecords[0].drawType	= DRT_IMAGE;
	atDrawRecords[0].colorValue	= 0x30FFFFFF;
	atDrawRecords[0].srcSize 	= _sizeSrcImage;

	// Union
	atDrawRecords[0].img.colorFormat = _eCFSrcImage;
	atDrawRecords[0].img.pSrc	     = _pImage;
	atDrawRecords[0].img.flag	     = 0;

	_DrawRecords( _rc3DCanvas, 0.9f, 1, &atDrawRecords[0] );

#ifdef _VT
//	m_csVideoSize = _sizeSrcImage; /// needed for 3d grid rendering
#endif // _VT

	return TRUE;
}

BOOL CD3d::m_DrawBitmap( int iCnt )
{
	int i;
	BYTE	*sTmpSurfac;

	DDSURFACEDESC2	ddsd; _INIT_DIRECTDRAW_STRUCT( ddsd );
	ddsd.dwSize = sizeof(ddsd);

	if(m_pddsTexture->Lock( NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL )!=DD_OK){
//		TRACE("CD3d::m_DrawBitmap Lock Error...\n");
		return FALSE;
	}

	sTmpSurfac = (BYTE *)ddsd.lpSurface;

	if(iCnt>MAXBMPICON)
		iCnt = MAXBMPICON;

	int iBitLen = 4;
	if(m_eD3dTextureCF==CF_BGR565||m_eD3dTextureCF==CF_YUY2)
		iBitLen = 2;

	for(i=0;i<iCnt;i++){
		if(_ConvertColorFormat( tMyIcon[i]._pImage, tMyIcon[i]._eCFSrcImage, tMyIcon[i]._sizeSrcImage, &sTmpSurfac[i*tMyIcon[i]._sizeSrcImage.cx*iBitLen], m_eD3dTextureCF, ddsd.lPitch, 1 )==FALSE){
//			TRACE("CD3d::m_DrawBitmap _ConvertColorFormat Error [%d]...\n", i);
			m_pddsTexture->Unlock(NULL);
			return FALSE;
		}
	}

	i--;
#if 1
	RECT dstRect = { tMyIcon[i]._rc3DCanvas.left-(tMyIcon[i]._sizeSrcImage.cx*iCnt), tMyIcon[i]._rc3DCanvas.top, tMyIcon[i]._rc3DCanvas.left, tMyIcon[i]._rc3DCanvas.bottom};
	RECT srcRect = { 0, 0, tMyIcon[i]._sizeSrcImage.cx*iCnt, tMyIcon[i]._sizeSrcImage.cy };
#else
	RECT dstRect = { 0, 10, tMyIcon[i]._sizeSrcImage.cx + i*tMyIcon[i]._sizeSrcImage.cx*2, tMyIcon[i]._sizeSrcImage.cy+10 };
	RECT srcRect = { 0, 0, tMyIcon[i]._sizeSrcImage.cx + i*tMyIcon[i]._sizeSrcImage.cx*2, tMyIcon[i]._sizeSrcImage.cy };
#endif
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	_DrawImageTexture( dstRect, srcRect, 0x30FFFFFF, 0.9f );

	m_pddsTexture->Unlock(NULL);
	return TRUE;
}

BOOL CD3d::m_DrawRectList( RECT			_rc3DCanvas,
						   SIZE			_sizeSrcImage,
						   RECT* const	_apRcList,
						   INT			_nRectCount,
						   BYTE			_cbAlpha,
						   COLORREF		_clrRect )
{
	if( 0 < _nRectCount ) {;} else return FALSE;
	if( _apRcList == NULL ) return FALSE;

	DWORD	dwColor = 0;
	dwColor = (DWORD)_clrRect;
	dwColor |= (DWORD)(_cbAlpha << 24);

	tDRAWRECORD	atDrawRecords[1];
	ZeroMemory( atDrawRecords, sizeof(atDrawRecords) );

	atDrawRecords[0].drawType	   = DRT_RECTLIST;
	atDrawRecords[0].colorValue	   = dwColor;
	atDrawRecords[0].srcSize 	   = _sizeSrcImage;

	// Union
	atDrawRecords[0].rects.nRect   = _nRectCount;
	atDrawRecords[0].rects.rects   = _apRcList;

	_DrawRecords( _rc3DCanvas, 0.9f, 1, atDrawRecords );

	return TRUE;
}

BOOL CD3d::m_DrawPolygonList( RECT				_rc3DCanvas,
						      SIZE				_sizeSrcImage,
						      MYPOLYGON* const	_aptPolygonList,
						      INT				_nPolygonCount,
#ifdef _VT
							eBRUSHTYPE			_brushType,
#endif // _VT
						      BYTE				_cbAlpha,
						      COLORREF			_clrRect )
{
	if( 0 < _nPolygonCount ) {;} else return FALSE;
	if( _aptPolygonList == NULL ) return FALSE;

	DWORD	dwColor = 0;
	dwColor = (DWORD)_clrRect;
	dwColor |= (DWORD)(_cbAlpha << 24);

	tDRAWRECORD	atDrawRecords[1];
	ZeroMemory( atDrawRecords, sizeof(atDrawRecords) );

	atDrawRecords[0].drawType	   = DRT_POLYGONLIST;
	atDrawRecords[0].colorValue	   = dwColor;
	atDrawRecords[0].srcSize 	   = _sizeSrcImage;

	// Union
	atDrawRecords[0].polygons.nPolygon   = _nPolygonCount;
	atDrawRecords[0].polygons.aptPolygon = _aptPolygonList;

#ifdef _VT
	atDrawRecords[0].brushType = _brushType;
#endif // _VT

#ifdef _DRAWRECORDS_ALL
	_PushRecord( _rc3DCanvas, atDrawRecords );
#else
	_DrawRecords( _rc3DCanvas, 0.9f, 1, atDrawRecords );
#endif

	return TRUE;
}

BOOL CD3d::m_DrawLineList( RECT			_rc3DCanvas,
						   SIZE			_sizeSrcImage,
						   POINT* const	_apPtList,
						   INT			_nPtCount,
						   BYTE			_cbAlpha,
						   COLORREF		_clrLine )
{
	if( 1 < _nPtCount ) {;} else return FALSE;
	if( _apPtList == NULL ) return FALSE;

	DWORD	dwColor = 0;
	dwColor = (DWORD)_clrLine;
	dwColor |= (DWORD)(_cbAlpha << 24);

	tDRAWRECORD	atDrawRecords[1];
	ZeroMemory( atDrawRecords, sizeof(atDrawRecords) );

	atDrawRecords[0].drawType	= DRT_LINESTRIP;
	atDrawRecords[0].colorValue	= dwColor;
	atDrawRecords[0].srcSize 	= _sizeSrcImage;

	// Union
	atDrawRecords[0].pts.nPt	= _nPtCount;
	atDrawRecords[0].pts.pts	= _apPtList;

#ifdef _DRAWRECORDS_ALL
	_PushRecord( _rc3DCanvas, atDrawRecords );
#else
	_DrawRecords( _rc3DCanvas, 0.9f, 1, atDrawRecords );
#endif


	return TRUE;
}

BOOL CD3d::m_DrawText( RECT			_rc3DCanvas,
					   SIZE			_sizeSrcImage,
					   POINT        _ptStartXY,
					   LPCSTR       _lpszText,
					   BYTE			_cbAlpha,
					   COLORREF		_clrFont
#ifdef _VT
					   ,float		_fScale
#endif // _VT
						)
{
	if( _lpszText == NULL ) return FALSE;
	if( 0 < lstrlen(_lpszText) ) {;} else return FALSE;

	DWORD	dwColor = 0;
	dwColor = (DWORD)_clrFont;
	dwColor |= (DWORD)(_cbAlpha << 24);

	tDRAWRECORD	atDrawRecords[1];
	ZeroMemory( atDrawRecords, sizeof(atDrawRecords) );

	atDrawRecords[0].drawType	= DRT_TEXT;
	atDrawRecords[0].colorValue	= dwColor;
	atDrawRecords[0].srcSize 	= _sizeSrcImage;

	// Union
	atDrawRecords[0].text.pText	= _lpszText;
	atDrawRecords[0].text.pt	= _ptStartXY;
#ifdef _VT
	atDrawRecords[0].text.fScale = _fScale;
#endif // _VT

#ifdef _DRAWRECORDS_ALL
	_PushRecord( _rc3DCanvas, atDrawRecords );
#else
	_DrawRecords( _rc3DCanvas, 0.9f, 1, atDrawRecords );
#endif

	return TRUE;
}

#ifdef _DRAWRECORDS_ALL
BOOL CD3d::m_DrawAllRecords()
{
	_DrawRecords( m_rcDrawingCanvas, 0.9f, m_nNumRecords, m_pRecords );
	// Clear all saved records
	_ClearAllRecords();
	return TRUE;
}

BOOL CD3d::_PushRecord( RECT rcCanvas, tDRAWRECORD *pRecord )
{
	// Drawing records buffer full?
	if ( m_nNumRecords >= MAX_NUM_RECORDS )
		m_DrawAllRecords();

	// Canvas always same? Yes, currently the canvas for all records are same
	// Maybe fix it later on
	m_rcDrawingCanvas = rcCanvas;
	m_pRecords[m_nNumRecords] = *pRecord;

	// Some drawing types used pointers, we need to copy all these to the buffers
	switch ( pRecord->drawType )
	{
	case DRT_TEXT:
		{
			TCHAR *pTextNext = m_pTexts[m_nNumTexts] + lstrlen( pRecord->text.pText ) + 1; // do not forget the last '\0'
			// Check text buffer overflow
			if ( pTextNext > m_pTexts[MAX_NUM_RECORDS] )
			{
				m_DrawAllRecords();
				pTextNext = m_pTexts[m_nNumTexts] + lstrlen( pRecord->text.pText ) + 1;
			}
			// Copy text to the text buffer
			lstrcpy( m_pTexts[m_nNumTexts], pRecord->text.pText );
			// Change the text pointer inside saved record
			m_pRecords[m_nNumRecords].text.pText = m_pTexts[m_nNumTexts];
			// Set the text pointer for next one
			m_pTexts[m_nNumTexts+1] = pTextNext;	
			m_nNumTexts++;
		}
		break;

	case DRT_POLYGONLIST:
		{
			MYPOLYGON *pPolygonNext = m_pPolygons[m_nNumPolygons] + pRecord->polygons.nPolygon;
			// Check polygon buffer overflow
			if ( pPolygonNext > m_pPolygons[MAX_NUM_RECORDS] )
			{
				m_DrawAllRecords();
				pPolygonNext = m_pPolygons[m_nNumPolygons] + pRecord->polygons.nPolygon;
			}
			// Copy polygons to the polygon buffer
			memcpy( m_pPolygons[m_nNumPolygons], pRecord->polygons.aptPolygon, sizeof(MYPOLYGON) * pRecord->polygons.nPolygon );
			// Change the polygon pointer inside saved record
			m_pRecords[m_nNumRecords].polygons.aptPolygon = m_pPolygons[m_nNumPolygons];
			// Set the polygon pointer for next one
			m_pPolygons[m_nNumPolygons+1] = pPolygonNext;
			m_nNumPolygons++;
		}
		break;

	case DRT_LINESTRIP:
		{
			POINT *pLinePointsNext = m_pLinePoints[m_nNumLines] + pRecord->pts.nPt;
			// Check linepoints buffer overflow
			if ( pLinePointsNext > m_pLinePoints[MAX_NUM_RECORDS] )
			{
				m_DrawAllRecords();
				pLinePointsNext = m_pLinePoints[m_nNumLines] + pRecord->pts.nPt;			
			}
			// Copy points to the linepoints buffer
			memcpy( m_pLinePoints[m_nNumLines], pRecord->pts.pts, sizeof(POINT) * pRecord->pts.nPt );
			// Change the pt pointer inside saved record
			m_pRecords[m_nNumRecords].pts.pts = m_pLinePoints[m_nNumLines];
			// Set the linepoints pointer for next one
			m_pLinePoints[m_nNumLines+1] = pLinePointsNext;
			m_nNumLines++;
		}
		break;
	}
	m_nNumRecords++;

	return TRUE;
}

BOOL CD3d::_ClearAllRecords()
{
	m_nNumRecords	= 0;
	m_nNumPolygons	= 0;
	m_nNumTexts		= 0;
	m_nNumLines		= 0;
	return TRUE;
}
#endif

BOOL CD3d::_DrawRecords( RECT			_rc3DCanvas,
						 D3DVALUE		_foZValue,
						 ULONG			_nRecordCount,
						 tDRAWRECORD*	_aptDrawRecords )
{
	BOOL	fRet = TRUE;

	m_pd3dDevice->BeginScene();
	EnterCriticalSection( &m_crit );

	for( ULONG i=0; i<_nRecordCount; i++ )
	{
		switch( _aptDrawRecords->drawType )
		{
		case DRT_IMAGE:		
			{
				/*
				이 단계에서는 3DCanvas(3DDevice Attached BackBuffer)에 뿌리기전 단계로
				입력 영상에 맞는 적당한 Texture Surface를 선정하고 이 Surface Pointer에
				실제 영상 데이터와 Texture Buffer간 1:1로 옮겨 넣는 부분이다.
				_ConvertColorFormat이라는 놈이 nDownScale_factor를 적용해주기는 하나,
				1:1로 할려고 한다.
				*/
				int	nDownScale_factor = 1;	// 1:1

				int srW = _aptDrawRecords->srcSize.cx;	// Source Image Width
				int srH = _aptDrawRecords->srcSize.cy;	// Source Image Height

				// 1:1로 뿌리고자 적당한 Texture Surface를 찾는다. (m_pddsTexture결정)
				// 결과는 m_dwTextureWidth, m_dwTextureHeight, m_pddsTexture를 사용한다.
				if( _FindSuitableTextureSurface( srW, srH ) == FALSE ) {
					//ErrDbg(1, "FAIL!! - _FindSuitableTextureSurface (find %dx%d) \r\n", srW, srH);
					break;
				}

				BYTE*	m_pSurf = NULL;
				DWORD	m_dwPitch = 0;
				RECT	rcSrc;
				SetRect( &rcSrc, 0, 0, 1, 1 );

				if( _GetSurfacePointer( m_pddsTexture, NULL, &m_pSurf, &m_dwPitch ) )
				{
#ifdef _VT
					if (!m_bVideoPaused) /// if video is paused, dont update the texture
					{
#endif //_VT
					// _aptDrawRecords->img.pSrc가 NULL인 경우
					// 현재 TextureSurface의 내용을 그대로 유지 하려고 하는 의도도 있다.
					_ConvertColorFormat( _aptDrawRecords->img.pSrc,
										 _aptDrawRecords->img.colorFormat,
										 _aptDrawRecords->srcSize,
										 m_pSurf,
										 m_eD3dTextureCF,
										 m_dwPitch,
										 nDownScale_factor );
#ifdef _VT
					}
#endif //_VT

					_ReleaseSurfacePointer( m_pddsTexture );

					m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );

					RECT srcRect = { 0, 0, srW, srH };
					_DrawImageTexture( _rc3DCanvas, srcRect, _aptDrawRecords->colorValue, _foZValue );
				}
				else {
					fRet = FALSE;
				}
			}
			break;
		case DRT_BITMAP:
			{				
/*				RECT	rcSrc;
				SetRect( &rcSrc, 0, 0, _aptDrawRecords->srcSize.cx, _aptDrawRecords->srcSize.cy );

				DDSURFACEDESC2	ddsd; _INIT_DIRECTDRAW_STRUCT( ddsd );
				ddsd.dwSize = sizeof(ddsd);

				if(m_pddsTexture->Lock( &rcSrc, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL )!=DD_OK)
					break;

//				if( _GetSurfacePointer( m_pddsTexture, &rcSrc, &m_pSurf, &m_dwPitch ) ){				
					_ConvertColorFormat( _aptDrawRecords->img.pSrc, _aptDrawRecords->img.colorFormat, _aptDrawRecords->srcSize, m_pSurf, m_eD3dTextureCF, ddsd.lPitch, 1 );

					m_pddsTexture->Unlock(&rcSrc);

					m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );	
					RECT srcRect = { 0, 0, _aptDrawRecords->srcSize.cx, _aptDrawRecords->srcSize.cy };
					_DrawImageTexture( _rc3DCanvas, srcRect, _aptDrawRecords->colorValue, _foZValue );
//				}
*/	
			}
			break;
		case DRT_POINTLIST:
		case DRT_LINELIST:
		case DRT_LINESTRIP:
			{
				//DrawPrimitive...
				POINT *pPt = _aptDrawRecords->pts.pts;

				int trW = _rc3DCanvas.right  - _rc3DCanvas.left;
				int trH = _rc3DCanvas.bottom - _rc3DCanvas.top;
				int srW = _aptDrawRecords->srcSize.cx;
				int srH = _aptDrawRecords->srcSize.cy;

				ULONG n = 0;
				for( ULONG i=0; i < _aptDrawRecords->pts.nPt; i++ ) {
					//clip rect..
					POINT pt = *pPt;

					pt.x = max( 0, pt.x );
					pt.x = min( _aptDrawRecords->srcSize.cx, pt.x );
					pt.y = max( 0, pt.y );
					pt.y = min( _aptDrawRecords->srcSize.cy, pt.y );

					m_aptRectAndLineTLV[n].dvSX = (float)(pt.x * trW/srW + _rc3DCanvas.left);
					m_aptRectAndLineTLV[n].dvSY = (float)(pt.y * trH/srH + _rc3DCanvas.top);

					n++;
					pPt++;
				}

				_SetVertexColorValue( m_aptRectAndLineTLV, 0, n, _aptDrawRecords->colorValue );
				_SetVertexZValue( m_aptRectAndLineTLV, 0, n, _foZValue );
				_EnableAlpha();

				m_pd3dDevice->DrawPrimitive( (D3DPRIMITIVETYPE)_aptDrawRecords->drawType, D3DFVF_TLVERTEX, m_aptRectAndLineTLV, n, NULL );
			}
			break;

		case DRT_POLYGONLIST:
			{

#ifdef _VT
				_DrawPolygonVCA( _rc3DCanvas, _foZValue, _nRecordCount, _aptDrawRecords );
#else
				//DrawPrimitive...
				MYPOLYGON* aptPoly = _aptDrawRecords->polygons.aptPolygon;

				int trW = _rc3DCanvas.right  - _rc3DCanvas.left;
				int trH = _rc3DCanvas.bottom - _rc3DCanvas.top;
				int srW = _aptDrawRecords->srcSize.cx;
				int srH = _aptDrawRecords->srcSize.cy;

				ULONG n = 0;
				for( ULONG i=0; i < _aptDrawRecords->polygons.nPolygon; i++ ) {
					//clip rect..
					MYPOLYGON polygon = *aptPoly;

					for( ULONG j=0; j < polygon.cnt_point - 2; j++ ) {
						POINT pt0 = polygon.atpoint[0];
						POINT pt1 = polygon.atpoint[j+1];
						POINT pt2 = polygon.atpoint[j+2];

						pt0.x = max( 0, pt0.x );
						pt0.x = min( _aptDrawRecords->srcSize.cx, pt0.x );
						pt0.y = max( 0, pt0.y );
						pt0.y = min( _aptDrawRecords->srcSize.cy, pt0.y );

						pt1.x = max( 0, pt1.x );
						pt1.x = min( _aptDrawRecords->srcSize.cx, pt1.x );
						pt1.y = max( 0, pt1.y );
						pt1.y = min( _aptDrawRecords->srcSize.cy, pt1.y );

						pt2.x = max( 0, pt2.x );
						pt2.x = min( _aptDrawRecords->srcSize.cx, pt2.x );
						pt2.y = max( 0, pt2.y );
						pt2.y = min( _aptDrawRecords->srcSize.cy, pt2.y );

						// Triangle Vertex를 만든다.
						// 뿌릴 사각형을 삼각형 여러개로 표현한다.

						m_aptRectAndLineTLV[n+0].dvSX = (float)(pt0.x * trW/srW + _rc3DCanvas.left);
						m_aptRectAndLineTLV[n+0].dvSY = (float)(pt0.y * trH/srH + _rc3DCanvas.top);
						m_aptRectAndLineTLV[n+1].dvSX = (float)(pt1.x * trW/srW + _rc3DCanvas.left);
						m_aptRectAndLineTLV[n+1].dvSY = (float)(pt1.y * trH/srH + _rc3DCanvas.top);
						m_aptRectAndLineTLV[n+2].dvSX = (float)(pt2.x * trW/srW + _rc3DCanvas.left);
						m_aptRectAndLineTLV[n+2].dvSY = (float)(pt2.y * trH/srH + _rc3DCanvas.top);
						m_aptRectAndLineTLV[n+3].dvSX = (float)(pt0.x * trW/srW + _rc3DCanvas.left);
						m_aptRectAndLineTLV[n+3].dvSY = (float)(pt0.y * trH/srH + _rc3DCanvas.top);
						
					//	m_aptRectAndLineTLV[n+0].dvSZ = -10;
					//	m_aptRectAndLineTLV[n+1].dvSZ = -10;
					//	m_aptRectAndLineTLV[n+2].dvSZ = -10;
					//	m_aptRectAndLineTLV[n+3].dvSZ = -10;
						n += 4;
					}

					aptPoly++;
				}
				_SetVertexColorValue( m_aptRectAndLineTLV, 0, n, _aptDrawRecords->colorValue );
				_SetVertexZValue( m_aptRectAndLineTLV, 0, n, _foZValue );
				_EnableAlpha();

				m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_aptRectAndLineTLV, n, NULL );
				// just test
				//m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, D3DFVF_TLVERTEX, m_aptRectAndLineTLV, n, NULL );
#endif // _VT
			}
			break;

		case DRT_RECTLIST:
			{
				//DrawPrimitive...
				RECT *pRc = _aptDrawRecords->rects.rects;

				int trW = _rc3DCanvas.right  - _rc3DCanvas.left;
				int trH = _rc3DCanvas.bottom - _rc3DCanvas.top;
				int srW = _aptDrawRecords->srcSize.cx;
				int srH = _aptDrawRecords->srcSize.cy;

				ULONG n = 0;
				for( ULONG i=0; i < _aptDrawRecords->rects.nRect; i++ )
				{
					//clip rect..
					RECT rc = *pRc;

					rc.left   = max( 0, rc.left );
					rc.top    = max( 0, rc.top );
					rc.right  = min( _aptDrawRecords->srcSize.cx, rc.right );
					rc.bottom = min( _aptDrawRecords->srcSize.cy, rc.bottom );

					// Triangle Vertext를 만든다.
					// 뿌릴 사각형을 삼각형 두개로 표현한다.

					m_aptRectAndLineTLV[n+0].dvSX = (float)(rc.left  * trW/srW + _rc3DCanvas.left);
					m_aptRectAndLineTLV[n+0].dvSY = (float)(rc.top   * trH/srH + _rc3DCanvas.top);
					m_aptRectAndLineTLV[n+1].dvSX = (float)(rc.right * trW/srW + _rc3DCanvas.left);
					m_aptRectAndLineTLV[n+1].dvSY = (float)(rc.bottom* trH/srH + _rc3DCanvas.top);
					m_aptRectAndLineTLV[n+2].dvSX = (float)(rc.left  * trW/srW + _rc3DCanvas.left);
					m_aptRectAndLineTLV[n+2].dvSY = (float)(rc.bottom* trH/srH + _rc3DCanvas.top);
					m_aptRectAndLineTLV[n+3].dvSX = (float)(rc.left  * trW/srW + _rc3DCanvas.left);
					m_aptRectAndLineTLV[n+3].dvSY = (float)(rc.top   * trH/srH + _rc3DCanvas.top);
					m_aptRectAndLineTLV[n+4].dvSX = (float)(rc.right * trW/srW + _rc3DCanvas.left);
					m_aptRectAndLineTLV[n+4].dvSY = (float)(rc.top   * trH/srH + _rc3DCanvas.top);
					m_aptRectAndLineTLV[n+5].dvSX = (float)(rc.right * trW/srW + _rc3DCanvas.left);
					m_aptRectAndLineTLV[n+5].dvSY = (float)(rc.bottom* trH/srH + _rc3DCanvas.top);
				
					n += 6;
					pRc++;
				}

				_SetVertexColorValue( m_aptRectAndLineTLV, 0, n, _aptDrawRecords->colorValue );
				_SetVertexZValue( m_aptRectAndLineTLV, 0, n, _foZValue );
				_EnableAlpha();

				m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, m_aptRectAndLineTLV, n, NULL );
			}
			break;

		case DRT_TEXT://text
			{
				if( _aptDrawRecords->text.pText && _aptDrawRecords->text.pText[0] )
				{
					int trW = _rc3DCanvas.right  - _rc3DCanvas.left;
					int trH = _rc3DCanvas.bottom - _rc3DCanvas.top;
					int srW = _aptDrawRecords->srcSize.cx;
					int srH = _aptDrawRecords->srcSize.cy;

					POINT pt = _aptDrawRecords->text.pt;
					pt.x = pt.x * trW/srW + _rc3DCanvas.left;
					pt.y = pt.y * trH/srH + _rc3DCanvas.top;

					_DrawText( (float)pt.x,
							   (float)pt.y,
							   _rc3DCanvas,
							   _aptDrawRecords->colorValue,
							   _aptDrawRecords->text.pText
#ifdef _VT
								, _aptDrawRecords->text.fScale 
#endif // _VT
								);
				}
			}
			break;

#ifdef _VT
		case DRT_CIRCLE: //circle
			{
				int  i;
				int  t1;
				POINT p1,p2;
				POINT centre = _aptDrawRecords->circle.ptCentre;
				double radius = _aptDrawRecords->circle.fRadius;
				BOOL fill =  _aptDrawRecords->circle.bFill;
				int srW = _aptDrawRecords->srcSize.cx;
				int srH = _aptDrawRecords->srcSize.cy;
				int alpha =  _aptDrawRecords->colorValue>>24 ;
				MYPOLYGON ptPolygonList;
				tDRAWRECORD tRecord;

				// Do not clip it, sometimes it won't draw the zone/counter nodes when the zone/counter is too close to the edge
//				if ((centre.x-radius)<0 || (centre.x+radius)>srW || (centre.y-radius)<0 ||(centre.y+radius)>srH)
//					return -1;
				
				memcpy( &tRecord, _aptDrawRecords, sizeof( tDRAWRECORD ) );
				tRecord.polygons.aptPolygon = &ptPolygonList;
				tRecord.polygons.nPolygon = 1;

				
				// first 3 points
				{
					p1.x = centre.x - radius;
					p1.y = centre.y;
					ptPolygonList.atpoint[0] = p1;

					i = centre.x - radius + 1;
					p1.x = i;
					p2.x = i;
					t1 = sqrt(radius*radius-(i-centre.x)*(i-centre.x));
					p1.y = centre.y - t1;
					p2.y = centre.y + t1;
					ptPolygonList.atpoint[1] = p1;
					ptPolygonList.atpoint[2] = p2;
					ptPolygonList.cnt_point = 3;

					if ( ( p1.x != p2.x ) || ( p1.y != p2.y ) )
						_DrawPolygonVCA( _rc3DCanvas, _foZValue, 1, &tRecord );
//					m_DrawPolygonList( _rc3DCanvas, _aptDrawRecords->srcSize, &ptPolygonList, 1, alpha, _aptDrawRecords->colorValue - (alpha<<24) );
				}

				// Middle part
				ptPolygonList.cnt_point = 4;
				ptPolygonList.atpoint[2] = p1;
				ptPolygonList.atpoint[3] = p2;
				for (i = centre.x - radius + 2; i < centre.x + radius; i++)
				{	
					p1.x = i;
					p2.x = i;
					t1 = sqrt(radius*radius-(i-centre.x)*(i-centre.x));
					p1.y = centre.y - t1;
					p2.y = centre.y + t1;

					ptPolygonList.atpoint[0] = ptPolygonList.atpoint[3];
					ptPolygonList.atpoint[1] = ptPolygonList.atpoint[2];

					ptPolygonList.atpoint[2] = p1;
					ptPolygonList.atpoint[3] = p2;
					_DrawPolygonVCA( _rc3DCanvas, _foZValue, 1, &tRecord );
//					m_DrawPolygonList( _rc3DCanvas, _aptDrawRecords->srcSize, &ptPolygonList, 1, alpha, _aptDrawRecords->colorValue - (alpha<<24) );
				}

				// last 3 points
				{
					p1.x = centre.x + radius;
					p1.y = centre.y;
					ptPolygonList.atpoint[2] = p1;

					i = centre.x + radius - 1;
					p1.x = i;
					p2.x = i;
					t1 = sqrt(radius*radius-(i-centre.x)*(i-centre.x));
					p1.y = centre.y - t1;
					p2.y = centre.y + t1;
					ptPolygonList.atpoint[0] = p2;
					ptPolygonList.atpoint[1] = p1;
					ptPolygonList.cnt_point = 3;

					if ( ( p1.x != p2.x ) || ( p1.y != p2.y ) )
						_DrawPolygonVCA( _rc3DCanvas, _foZValue, 1, &tRecord );
				}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
				// first 3 points
				{
					p1.x = centre.x;
					p1.y = centre.y - radius;
					ptPolygonList.atpoint[0] = p1;

					i =  centre.y - radius + 1;
					p1.y = i;
					p2.y = i;
					t1 = sqrt(radius*radius-(i-centre.y)*(i-centre.y));
					p1.x = centre.x - t1;
					p2.x = centre.x + t1;
					ptPolygonList.atpoint[1] = p1;
					ptPolygonList.atpoint[2] = p2;
					ptPolygonList.cnt_point = 3;
					if ( ( p1.x != p2.x ) || ( p1.y != p2.y ) )
						_DrawPolygonVCA( _rc3DCanvas, _foZValue, 1, &tRecord );
	//					m_DrawPolygonList( _rc3DCanvas, _aptDrawRecords->srcSize, &ptPolygonList, 1, alpha, _aptDrawRecords->colorValue - (alpha<<24) );
				}
				
				// Middle part
				ptPolygonList.cnt_point = 4;
				ptPolygonList.atpoint[2] = p1;
				ptPolygonList.atpoint[3] = p2;
				for (i = centre.y - radius + 2; i < centre.y + radius; i++)
				{	
					p1.y = i;
					p2.y = i;
					t1 = sqrt(radius*radius-(i-centre.y)*(i-centre.y));
					p1.x = centre.x - t1;
					p2.x = centre.x + t1;

					ptPolygonList.atpoint[0] = ptPolygonList.atpoint[3];
					ptPolygonList.atpoint[1] = ptPolygonList.atpoint[2];

					ptPolygonList.atpoint[2] = p1;
					ptPolygonList.atpoint[3] = p2;
					_DrawPolygonVCA( _rc3DCanvas, _foZValue, 1, &tRecord );
//					m_DrawPolygonList( _rc3DCanvas, _aptDrawRecords->srcSize, &ptPolygonList, 1, alpha, _aptDrawRecords->colorValue - (alpha<<24) );
				}
				// last 3 points
				{
					p1.x = centre.x;
					p1.y = centre.y + radius;
					ptPolygonList.atpoint[2] = p1;

					i =  centre.y - radius + 1;
					p1.y = i;
					p2.y = i;
					t1 = sqrt(radius*radius-(i-centre.y)*(i-centre.y));
					p1.x = centre.x - t1;
					p2.x = centre.x + t1;
					ptPolygonList.atpoint[1] = p1;
					ptPolygonList.atpoint[2] = p2;
					ptPolygonList.cnt_point = 3;
					if ( ( p1.x != p2.x ) || ( p1.y != p2.y ) )
						_DrawPolygonVCA( _rc3DCanvas, _foZValue, 1, &tRecord );
	//					m_DrawPolygonList( _rc3DCanvas, _aptDrawRecords->srcSize, &ptPolygonList, 1, alpha, _aptDrawRecords->colorValue - (alpha<<24) );
				}
			}
			break;
		//______________________________________________________
		//	draw a circular arc between the two small arrows for zone stuff.
		case DRT_CIRCULARARC:
			{
				CD3d::MYPOLYGON ptPolygonList;
				tDRAWRECORD tRecord;
				POINT		   centre  = _aptDrawRecords->circularArc.ptCentre;
				double		   radius  = _aptDrawRecords->circularArc.fRadius;
				double		   dStartRadius  = _aptDrawRecords->circularArc.dStartRadius;
				double		   dFinishRadius = _aptDrawRecords->circularArc.dFinishRadius;
				unsigned short usWidth = _aptDrawRecords->circularArc.usWidth;
				unsigned short usHalfWidth = (float)(usWidth - 1)*0.5;

				int srW = _aptDrawRecords->srcSize.cx;
				int srH = _aptDrawRecords->srcSize.cy;
				int alpha =  _aptDrawRecords->colorValue>>24 ;
				
				double dCurrentRadius = dStartRadius;

				if ((centre.x-radius)<0 || (centre.x+radius)>srW || (centre.y-radius)<0 ||(centre.y+radius)>srH)
					return -1;

				memcpy( &tRecord, _aptDrawRecords, sizeof( tDRAWRECORD ) );
				tRecord.polygons.aptPolygon = &ptPolygonList;
				tRecord.polygons.nPolygon = 1;

				ptPolygonList.cnt_point = 4;

				while(dCurrentRadius + RADIUS_STEP < dFinishRadius)
				{
					ptPolygonList.atpoint[0].x =   ( radius - usHalfWidth ) * cos(dCurrentRadius) + centre.x;
					ptPolygonList.atpoint[0].y = - ( radius - usHalfWidth ) * sin(dCurrentRadius) + centre.y;		//-- "-" suggests the screen coordinates is different.

					ptPolygonList.atpoint[1].x =   ( radius + usHalfWidth ) * cos(dCurrentRadius) + centre.x;
					ptPolygonList.atpoint[1].y = - ( radius + usHalfWidth ) * sin(dCurrentRadius) + centre.y;
		
					ptPolygonList.atpoint[2].x =   ( radius + usHalfWidth ) * cos(dCurrentRadius + RADIUS_STEP) + centre.x;
					ptPolygonList.atpoint[2].y = - ( radius + usHalfWidth ) * sin(dCurrentRadius + RADIUS_STEP) + centre.y;
				
					ptPolygonList.atpoint[3].x =   ( radius - usHalfWidth ) * cos(dCurrentRadius + RADIUS_STEP) + centre.x;
					ptPolygonList.atpoint[3].y = - ( radius - usHalfWidth ) * sin(dCurrentRadius + RADIUS_STEP) + centre.y;

					_DrawPolygonVCA( _rc3DCanvas, _foZValue, 1, &tRecord );
//					m_DrawPolygonList( _rc3DCanvas, _aptDrawRecords->srcSize, &ptPolygonList, 1, alpha, _aptDrawRecords->colorValue - (alpha<<24) );

					dCurrentRadius += RADIUS_STEP;
				}

				ptPolygonList.atpoint[0].x =   ( radius - usHalfWidth ) * cos(dCurrentRadius - RADIUS_STEP) + centre.x;
				ptPolygonList.atpoint[0].y = - ( radius - usHalfWidth ) * sin(dCurrentRadius - RADIUS_STEP) + centre.y;

				ptPolygonList.atpoint[1].x =   ( radius + usHalfWidth ) * cos(dCurrentRadius - RADIUS_STEP) + centre.x;
				ptPolygonList.atpoint[1].y = - ( radius + usHalfWidth ) * sin(dCurrentRadius - RADIUS_STEP) + centre.y;
		
				ptPolygonList.atpoint[2].x =   ( radius + usHalfWidth ) * cos(dFinishRadius) + centre.x;
				ptPolygonList.atpoint[2].y = - ( radius + usHalfWidth ) * sin(dFinishRadius) + centre.y;
				
				ptPolygonList.atpoint[3].x =   ( radius - usHalfWidth ) * cos(dFinishRadius) + centre.x;
				ptPolygonList.atpoint[3].y = - ( radius - usHalfWidth ) * sin(dFinishRadius) + centre.y;

				_DrawPolygonVCA( _rc3DCanvas, _foZValue, 1, &tRecord );
//				m_DrawPolygonList( _rc3DCanvas, _aptDrawRecords->srcSize, &ptPolygonList, 1, alpha, _aptDrawRecords->colorValue - (alpha<<24) );
			}
			break;
#endif // _VT

		}//switch( _aptDrawRecords->drawType )

		_aptDrawRecords++;

	}//for( ULONG i=0; i<_nRecordCount; i++ )
	
	LeaveCriticalSection( &m_crit );
	m_pd3dDevice->EndScene();

	return fRet;
}

BOOL CD3d::_DrawImageTexture( RECT _rc3DCanvas, RECT _rcSrc, DWORD _dwColor, D3DVALUE _foZValue )
{
	/*
	여기서는 이전 단계에서 만들어진 Texture Buffer의 내용을 3DCanvas(3DDevice Attached BackBuffer)
	로 옮긴다. 실제 3D Engine이 가동되고 ScaleUpDown도 발생한다.
	*/
	HRESULT hr;

	// 3DCanvas의 어디로 옮길것인지 결정한다.
	int x1 = _rc3DCanvas.left;
	int y1 = _rc3DCanvas.top;
	int x2 = _rc3DCanvas.right;
	int y2 = _rc3DCanvas.bottom;

	float fx1 = (float)x1 - 0.5f;
	float fy1 = (float)y1 - 0.5f;
	float fx2 = (float)x2 - 0.5f;
	float fy2 = (float)y2 - 0.5f;

	// 입력Texture 좌표.. 일단 pixel좌표를 쓴다.
	float u1 = (float)_rcSrc.left;
	float v1 = (float)_rcSrc.top;
	float u2 = (float)_rcSrc.right;
	float v2 = (float)_rcSrc.bottom;

	_SetVertexScreenXYZ( m_aptImageTLV, 0, 1, D3DVECTOR( fx1, fy2, _foZValue ) );
	_SetVertexScreenXYZ( m_aptImageTLV, 1, 1, D3DVECTOR( fx1, fy1, _foZValue ) );
	_SetVertexScreenXYZ( m_aptImageTLV, 2, 1, D3DVECTOR( fx2, fy2, _foZValue ) );
	_SetVertexScreenXYZ( m_aptImageTLV, 3, 1, D3DVECTOR( fx2, fy1, _foZValue ) );

	_SetVertexColorValue( m_aptImageTLV, 0, 4, _dwColor );

	u1 /= m_dwTextureWidth;
	v1 /= m_dwTextureHeight;
	u2 /= m_dwTextureWidth;
	v2 /= m_dwTextureHeight;
	_SetVertexTextureXY( m_aptImageTLV, 0, 1, u1, v2 );
	_SetVertexTextureXY( m_aptImageTLV, 1, 1, u1, v1 );
	_SetVertexTextureXY( m_aptImageTLV, 2, 1, u2, v2 );
	_SetVertexTextureXY( m_aptImageTLV, 3, 1, u2, v1 );

    // Draw the front and back faces of the cube using texture 1
	m_pd3dDevice->SetTexture( 0, m_pddsTexture );

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR );

    hr = m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_aptImageTLV, 4, NULL );

	return 0;
}

void CD3d::_SetVertexScreenXYZ( D3DTLVERTEX* _aptVertex, int indexFrom, int count, D3DVECTOR _tVector )
{
	for( int i = indexFrom; i < indexFrom + count; i++ ) {
		_aptVertex[i].dvSX = _tVector.dvX;
		_aptVertex[i].dvSY = _tVector.dvY;
		_aptVertex[i].dvSZ = _tVector.dvZ;
	}
}

void CD3d::_SetVertexScreenXY( D3DTLVERTEX* _aptVertex, int indexFrom, int count, D3DVALUE _foX, D3DVALUE _foY )
{
	for( int i = indexFrom; i < indexFrom + count; i++ ) {
		_aptVertex[i].dvSX = _foX;
		_aptVertex[i].dvSY = _foY;
	}
}

void CD3d::_SetVertexZValue( D3DTLVERTEX* _aptVertex, int indexFrom, int count, D3DVALUE _foZValue )
{
	for( int i = indexFrom; i < indexFrom + count; i++ ) {
		_aptVertex[i].dvSZ = _foZValue;
	}
}

void CD3d::_SetVertexColorValue( D3DTLVERTEX* _aptVertex, int indexFrom, int count, DWORD _dwColor )
{
	for( int i = indexFrom; i < indexFrom + count; i++ ) {
		_aptVertex[i].dcColor = _dwColor;
	}
}

void CD3d::_SetVertexTextureXY( D3DTLVERTEX* _aptVertex, int indexFrom, int count, D3DVALUE _foU, D3DVALUE _foV )
{
	for( int i = indexFrom; i < indexFrom + count; i++ ) {
		_aptVertex[i].dvTU = _foU;
		_aptVertex[i].dvTV = _foV;
	}
}

void CD3d::_EnableAlpha()
{
	m_pd3dDevice->SetTexture( 0, NULL );

	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,	TRUE );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND,			D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND,			D3DBLEND_INVSRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE,	TRUE );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF,			0x08 );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAFUNC,			D3DCMP_GREATEREQUAL );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FILLMODE,			D3DFILL_SOLID );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_CULLMODE,			D3DCULL_CCW );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE,			TRUE );

}

BOOL CD3d::_MakeStateBlock( LPDIRECTDRAWSURFACE7 const _pDDSurface )
{
	if(m_pd3dDevice == NULL)
	{
		return FALSE;
	}
	
	for( UINT which = 0; which < 2; which++ )
	{
		m_pd3dDevice->BeginStateBlock();
		
		m_pd3dDevice->SetTexture( 0, _pDDSurface );

		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,	TRUE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND,			D3DBLEND_SRCALPHA );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND,			D3DBLEND_INVSRCALPHA );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE,	TRUE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF,			0x08 );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAFUNC,			D3DCMP_GREATEREQUAL );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FILLMODE,			D3DFILL_SOLID );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_CULLMODE,			D3DCULL_CCW );

		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE,			TRUE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_STENCILENABLE,		FALSE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_CLIPPING,			TRUE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_EDGEANTIALIAS,		FALSE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_CLIPPLANEENABLE,	FALSE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_VERTEXBLEND,		FALSE );
		//m_pd3dDevice->SetRenderState( D3DRENDERSTATE_INDEXEDVERTEXBLENDENABLE, FALSE );//이 파라미터는 없는것 같다.
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGENABLE,			FALSE );

		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	 D3DTOP_MODULATE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,	 D3DTOP_MODULATE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_NONE);
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
		m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	 D3DTOP_DISABLE );
		m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,	 D3DTOP_DISABLE );

		if( which == 0 )
			m_pd3dDevice->EndStateBlock( &m_dwSavedStateBlock );
		else
			m_pd3dDevice->EndStateBlock( &m_dwDrawTextStateBlock );
	}

	return TRUE;
}

BOOL CD3d::_FindSuitableTextureSurface( INT _nTargetWidth, INT _nTargetHeight )
{
	BOOL	fFind = FALSE;

	int w = _nTargetWidth;
	int h = _nTargetHeight;

#if 0
	// 적당한것을 고른다.
	if     (w<= 256 && h<= 128){ m_pddsTexture = m_apddsTextures[0]; m_dwTextureWidth= 256;m_dwTextureHeight= 128; fFind = TRUE; }
	else if(w<= 256 && h<= 256){ m_pddsTexture = m_apddsTextures[1]; m_dwTextureWidth= 256;m_dwTextureHeight= 256; fFind = TRUE; }
	else if(w<= 512 && h<= 128){ m_pddsTexture = m_apddsTextures[2]; m_dwTextureWidth= 512;m_dwTextureHeight= 128; fFind = TRUE; }
	else if(w<= 512 && h<= 256){ m_pddsTexture = m_apddsTextures[3]; m_dwTextureWidth= 512;m_dwTextureHeight= 256; fFind = TRUE; }
	else if(w<= 512 && h<= 512){ m_pddsTexture = m_apddsTextures[4]; m_dwTextureWidth= 512;m_dwTextureHeight= 512; fFind = TRUE; }
	else if(w<=1024 && h<= 256){ m_pddsTexture = m_apddsTextures[5]; m_dwTextureWidth=1024;m_dwTextureHeight= 256; fFind = TRUE; }
	else if(w<=1024 && h<= 512){ m_pddsTexture = m_apddsTextures[6]; m_dwTextureWidth=1024;m_dwTextureHeight= 512; fFind = TRUE; }
	else 					   { m_pddsTexture = m_apddsTextures[7]; m_dwTextureWidth=1024;m_dwTextureHeight=1024; fFind = TRUE; }

#else
	//
	// VIA chipset 마더보드의 built-in VGA인 경우 1024 x 1024만 열린다.
	//
	// 가장 크게 잡은 1024만 사용한다.
	//
	fFind = TRUE;
	m_pddsTexture	   = m_apddsTextures[7];

	if( w > (INT)m_dwTextureWidth || h > (INT)m_dwTextureHeight ) {
		fFind = FALSE;
	}
#endif

	return fFind;
}

BOOL CD3d::_GetSurfacePointer( LPDIRECTDRAWSURFACE7 const _pDDSurface,
							   RECT* const				  _prcTarget,
							   BYTE**					  _ppSurf,
							   DWORD*					  _pdwPitch )
{
	DDSURFACEDESC2	ddsd; _INIT_DIRECTDRAW_STRUCT( ddsd );
	ddsd.dwSize = sizeof(ddsd);
	
	HRESULT hr = _pDDSurface->IsLost();
	if( hr == DDERR_SURFACELOST ) {
		hr = _pDDSurface->Restore();
		if( FAILED(hr) ) {
			_MACRO_DDERROR( hr, "Restore" );
			return FALSE;
		}
	}

	hr = _pDDSurface->Lock( _prcTarget,
							&ddsd, 
							DDLOCK_SURFACEMEMORYPTR |
							DDLOCK_WAIT |
							DDLOCK_NOSYSLOCK |
							DDLOCK_WRITEONLY,
							NULL );
	if( FAILED(hr) ) {
		_MACRO_DDERROR( hr, "Lock" );
		return FALSE;
	}

	*_ppSurf = (BYTE*)ddsd.lpSurface;
	*_pdwPitch = ddsd.lPitch;

	return TRUE;
}

BOOL CD3d::_ReleaseSurfacePointer( LPDIRECTDRAWSURFACE7 const _pDDSurface )
{
	HRESULT hr = _pDDSurface->Unlock( NULL );
	if( FAILED(hr) ) {
//		TRACE("=================>   Unlock Fail\n");
		return FALSE;
	}
	return TRUE;
}

void CD3d::_InitColorConversionFunctions()
{
	BOOL sse2_support = IsSSE2Supported();

	if( sse2_support ) {
		cc_YUY2_To_RGB32			= YUY2_To_RGB32			;
		cc_YUY2_To_RGB32_Resize_2	= YUY2_To_RGB32_Resize_2_SSE;//YUY2_To_RGB32_Resize_2;	
		cc_YUY2_To_RGB555			= YUY2_To_RGB555		;	
		cc_YUY2_To_RGB555_Resize_2	= YUY2_To_RGB555_Resize_2_2;
		cc_YUY2_To_RGB565			= YUY2_To_RGB565			;
		cc_YUY2_To_RGB565_Resize_2	= YUY2_To_RGB565_Resize_2_2;
		//cc_YUY2_To_RGB565_Resize_2	= YUY2_To_RGB565_Resize_2_SSE;
	}else{
		cc_YUY2_To_RGB32			= YUY2_To_RGB32_SSE			;
		cc_YUY2_To_RGB32_Resize_2	= YUY2_To_RGB32_Resize_2_SSE;	
		cc_YUY2_To_RGB555			= YUY2_To_RGB555_SSE		;	
		cc_YUY2_To_RGB555_Resize_2	= YUY2_To_RGB555_Resize_2_SSE;
		cc_YUY2_To_RGB565			= YUY2_To_RGB565_SSE		;	
		cc_YUY2_To_RGB565_Resize_2	= YUY2_To_RGB565_Resize_2_SSE;
	}
}

BOOL CD3d::_ConvertColorFormat( BYTE*		 _pSrc,
							    eCOLORFORMAT _eCFSource,
								SIZE		 _sizeSource,
								BYTE*		 _pDst,
								eCOLORFORMAT _eCFDest,
								DWORD		 _dwDstPitch,
								DWORD		 _dwScale_factor )
{
	if( _pSrc == NULL ) return FALSE;

	//_pDst의 BGR565는 모두 BGRX로 바뀔 예정
	const DWORD pixel_bytes[] = { 0,2,4,3,2,2 };	// bps
	//ErrDbg(1, "_ConvertColorFormat _pSrc %Xh, _pDst %Xh \r\n", _pSrc, _pDst);

	if( _eCFDest == _eCFSource )
	{
		if( _dwScale_factor == 1 ) {
/*			for(int i = 0; i < _sizeSource.cy; i++) {
				CopyMemory(_pDst + i*_dwDstPitch, _pSrc + (_sizeSource.cx*4)*i, _sizeSource.cx*4);
			}
*/			HSH_CopySameFormatImage( _pDst, _pSrc, _dwDstPitch, _sizeSource.cx, _sizeSource.cy, pixel_bytes[_eCFSource] );
			return TRUE;
		}
	}

	if( _eCFSource != CF_YUY2 &&
		_eCFSource != CF_YV12 &&
		_eCFSource != CF_IYUV    ) { return FALSE; }

	switch( _eCFSource )
	{
	case CF_YUY2:
		if( _eCFDest == CF_YUY2 )
		{
			if( _dwScale_factor == 1 ) { HSH_CopyYUYVImage   ( _pDst, _pSrc, _dwDstPitch, _sizeSource.cx, _sizeSource.cy ); }
			else
			if( _dwScale_factor == 2 ) { HSH_CopyYUYVImageBy2( _pDst, _pSrc, _dwDstPitch, _sizeSource.cx, _sizeSource.cy ); }
			else
			if( _dwScale_factor == 4 ) { HSH_CopyYUYVImageBy4( _pDst, _pSrc, _dwDstPitch, _sizeSource.cx, _sizeSource.cy ); }
			else { return FALSE; }
		}
		else
		if( _eCFDest == CF_BGR555 )
		{
			if( _dwScale_factor == 1 ) {
				if( _sizeSource.cx % 8 ) YUY2_To_RGB555_SSE(_pSrc, _pDst, _dwDstPitch, _sizeSource.cx, _sizeSource.cy );
				else					 cc_YUY2_To_RGB555( _pSrc, _pDst, _dwDstPitch, _sizeSource.cx, _sizeSource.cy );
			}
			else
			if( _dwScale_factor == 2 ) {
				cc_YUY2_To_RGB555_Resize_2( _pSrc, _pDst, _dwDstPitch, _sizeSource.cx, _sizeSource.cy );
			}
			else { return FALSE; }
		}
		else
		if( _eCFDest == CF_BGR565 )
		{
			if( _dwScale_factor == 1 ) {
				if( _sizeSource.cx % 8 ) YUY2_To_RGB565_SSE( _pSrc, _pDst, _dwDstPitch, _sizeSource.cx, _sizeSource.cy );
				else					 cc_YUY2_To_RGB565( _pSrc, _pDst, _dwDstPitch, _sizeSource.cx, _sizeSource.cy );
			}
			else
			if( _dwScale_factor == 2 ) {
				cc_YUY2_To_RGB565_Resize_2( _pSrc, _pDst, _dwDstPitch, _sizeSource.cx, _sizeSource.cy );
			}
			else { return FALSE; }
		}
		else
		if( _eCFDest == CF_BGRX )
		{
			if( _dwScale_factor == 1 ) {
				if( _sizeSource.cx % 8 )
					YUY2_To_RGB32_SSE( _pSrc, _pDst, _dwDstPitch, _sizeSource.cx, _sizeSource.cy );
				else
					cc_YUY2_To_RGB32( _pSrc, _pDst, _dwDstPitch, _sizeSource.cx, _sizeSource.cy );
			}
			else
			if( _dwScale_factor == 2 ) {
				cc_YUY2_To_RGB32_Resize_2(_pSrc,_pDst,_dwDstPitch,_sizeSource.cx,_sizeSource.cy);
			}
			else { return FALSE; }
		}
		else { return FALSE; }
		break;

	case CF_IYUV:
		if( _eCFDest == CF_BGR565 )
		{
			//포인터에 대한 추가적인 변수가 필요하다..
			BYTE* u_src = _pSrc+_sizeSource.cx * _sizeSource.cy;
			BYTE* v_src = _pSrc+_sizeSource.cx * _sizeSource.cy * 5 / 4;
			if( _dwScale_factor == 1 ) {
				YUY12_To_RGB565_mmx(_pDst,_dwDstPitch,_pSrc,u_src,v_src,_sizeSource.cx,_sizeSource.cx/2,_sizeSource.cx,_sizeSource.cy);
			}
			else
			if( _dwScale_factor == 2 ) {
				YUY12_To_RGB565_mmx_resize_2(_pDst,_dwDstPitch,_pSrc,u_src,v_src,_sizeSource.cx,_sizeSource.cx/2,_sizeSource.cx,_sizeSource.cy);
			}
			else { return FALSE; }
		}
		else { return FALSE; }
		break;

	case CF_YV12:
		if( _eCFDest == CF_BGR565 )
		{
			//포인터에 대한 추가적인 변수가 필요하다..
			BYTE* u_src = _pSrc+_sizeSource.cx * _sizeSource.cy;
			BYTE* v_src = _pSrc+_sizeSource.cx * _sizeSource.cy * 5 / 4;
			if( _dwScale_factor == 1 ) {
				YUY12_To_RGB565_mmx(_pDst,_dwDstPitch,_pSrc,u_src,v_src,_sizeSource.cx,_sizeSource.cx/2,_sizeSource.cx,_sizeSource.cy);
			}
			else
			if( _dwScale_factor== 2 ) {
				YUY12_To_RGB565_mmx_resize_2(_pDst,_dwDstPitch,_pSrc,u_src,v_src,_sizeSource.cx,_sizeSource.cx/2,_sizeSource.cx,_sizeSource.cy);
			}
			else{ return FALSE; }
		}
		else { return FALSE; }
		break;
	}//switch( _eCFSource )

	return TRUE;
}

HRESULT WINAPI CD3d::_lpEnumZBufferCallback( DDPIXELFORMAT* pddpf, VOID* pddpfDesired )
{
    // For this tutorial, we are only interested in z-buffers, so ignore any
    // other formats (e.g. DDPF_STENCILBUFFER) that get enumerated. An app
    // could also check the depth of the z-buffer (16-bit, etc,) and make a
    // choice based on that, as well. For this tutorial, we'll take the first
    // one we get.

#ifdef _VT // choose zbuffer with stencil buffer support to enable stencil buffer
    if( pddpf->dwFlags == (DDPF_ZBUFFER | DDPF_STENCILBUFFER))
    {
        memcpy( pddpfDesired, pddpf, sizeof(DDPIXELFORMAT) );
 
        // Return with D3DENUMRET_CANCEL to end the search.
       return D3DENUMRET_CANCEL;
    }
#else // _VT
	if( pddpf->dwFlags == DDPF_ZBUFFER)
    {
        memcpy( pddpfDesired, pddpf, sizeof(DDPIXELFORMAT) );
 
        // Return with D3DENUMRET_CANCEL to end the search.
       return D3DENUMRET_CANCEL;
    }
#endif

    // Return with D3DENUMRET_OK to continue the search.
    return D3DENUMRET_OK;
}

LPDIRECTDRAWSURFACE7 CD3d::_CreateCustomSurface( LPDIRECTDRAW7	_pDD,
												 eSURFACETYPE	_eSurfaceType,
												 PVOID			_lpParam,
												 DWORD			_dwWidth,
												 DWORD			_dwHeight )
{
	HRESULT hr;

	DDSURFACEDESC2	ddsd;
	_INIT_DIRECTDRAW_STRUCT( ddsd );

	TCHAR	szForDebug[128];

	switch(_eSurfaceType) {
	case SURFACETYPE_AGPTEXTURE: {
		wsprintf(szForDebug, "SURFACETYPE_AGPTEXTURE %dx%d", _dwWidth, _dwHeight);
		ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth         = _dwWidth;
		ddsd.dwHeight        = _dwHeight;
		ddsd.ddpfPixelFormat = *(DDPIXELFORMAT*)_lpParam;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE|DDSCAPS_VIDEOMEMORY|DDSCAPS_NONLOCALVIDMEM;
		break;
		}
	case SURFACETYPE_VIDMEMTEXTURE: {
		wsprintf(szForDebug, "SURFACETYPE_VIDMEMTEXTURE %dx%d", _dwWidth, _dwHeight);
		ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth         = _dwWidth;
		ddsd.dwHeight        = _dwHeight;
		ddsd.ddpfPixelFormat = *(DDPIXELFORMAT*)_lpParam;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE|DDSCAPS_VIDEOMEMORY|DDSCAPS_LOCALVIDMEM ;
		break;
		}
	case SURFACETYPE_MANAGEDTEXTURE: {
		wsprintf(szForDebug, "SURFACETYPE_MANAGEDTEXTURE %dx%d", _dwWidth, _dwHeight);
		ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_TEXTURESTAGE|DDSD_PIXELFORMAT;
		ddsd.dwWidth         = _dwWidth;
		ddsd.dwHeight        = _dwHeight;
		ddsd.ddpfPixelFormat = *(DDPIXELFORMAT*)_lpParam;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
		break;
		}
	case SURFACETYPE_3DCANVAS: {
		wsprintf(szForDebug, "SURFACETYPE_3DCANVAS %dx%d", _dwWidth, _dwHeight);
		ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
		ddsd.dwWidth		= _dwWidth;
		ddsd.dwHeight		= _dwHeight;
		ddsd.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE | DDSCAPS_LOCALVIDMEM ;//DDSCAPS_OFFSCREENPLAIN
		break;
		}
	case SURFACETYPE_PRIMARY: {
		lstrcpy(szForDebug, "SURFACETYPE_PRIMARY");
		ddsd.dwFlags        = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		break;
		}

	default:
		return NULL;
	}

	LPDIRECTDRAWSURFACE7  pddsTexture = NULL;
	do	// for easy return coding
	{
		hr = _pDD->CreateSurface( &ddsd, &pddsTexture, NULL );
		if( FAILED(hr) ) {
			pddsTexture = NULL;
			lstrcat( szForDebug, " CreateSurface" );
			_MACRO_DDERROR( hr, szForDebug );
			break;
		}

		if( _eSurfaceType == SURFACETYPE_3DCANVAS )
		{
			LPDIRECT3D7		pD3D;
			hr = _pDD->QueryInterface( IID_IDirect3D7, (PVOID*)&pD3D );
			if( FAILED(hr) ) {
				_SAFE_RELEASE( pddsTexture );
				_MACRO_DDERROR( hr, "IID_IDirect3D7 QueryInterface" );
				break;
			}

			DDPIXELFORMAT pixFormat;
			pD3D->EnumZBufferFormats( IID_IDirect3DHALDevice, 
									  _lpEnumZBufferCallback, 
									  &pixFormat );

			ddsd.dwFlags		 = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT|DDSD_PIXELFORMAT ;
			ddsd.dwWidth		 = _dwWidth;
			ddsd.dwHeight		 = _dwHeight;
			ddsd.ddpfPixelFormat = pixFormat;
			ddsd.ddsCaps.dwCaps  = DDSCAPS_ZBUFFER|DDSCAPS_VIDEOMEMORY;

			

			LPDIRECTDRAWSURFACE7  pddsZBuffer = NULL;
			hr = _pDD->CreateSurface( &ddsd, &pddsZBuffer, NULL );
			if( FAILED(hr) ) {
				_SAFE_RELEASE( pddsTexture );
				_MACRO_DDERROR( hr, "CreateSurface ZBuffer" );
				break;
			}
				
			hr = pddsTexture->AddAttachedSurface( pddsZBuffer );
			_SAFE_RELEASE( pddsZBuffer );
			if( FAILED(hr) ) {
				_SAFE_RELEASE( pddsTexture );
				_MACRO_DDERROR( hr, "AddAttachedSurface" );
				break;
			}

			LPDIRECT3DDEVICE7	pD3DDevice;
			hr = pD3D->CreateDevice( IID_IDirect3DHALDevice, pddsTexture, (LPDIRECT3DDEVICE7*)&pD3DDevice );
			if( FAILED(hr) ) {
				_SAFE_RELEASE( pddsTexture );
				_MACRO_DDERROR( hr, "CreateDevice IID_IDirect3DHALDevice" );
				break;
			}
			_SAFE_RELEASE( pD3D );

			pD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
			pD3DDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0, 1.0, 0 );
			*(LPDIRECT3DDEVICE7*)_lpParam = pD3DDevice;
			break;
		}//if( _eSurfaceType == SURFACETYPE_3DCANVAS )
	}while(0);

	return pddsTexture;
}

BOOL CD3d::m_CopyRectIn3DCanvas( RECT& rcNew, RECT& rcOld )
{
	m_pddsBackbuffer->Blt( &rcNew,
		                   m_pddsBackbuffer,
						   &rcOld,
						   DDBLT_ASYNC | DDBLT_DONOTWAIT,
						   NULL );
	return TRUE;
}

INT CD3d::m_GetAdapterNoByPosition( RECT const _rcScreen )
{
	INT	nNo = 0;

	for( INT i=1; i < m_nDDAdapterCount; i++ ) {
		HMONITOR hMonitor;
		MONITORINFO mi = {0,};
		mi.cbSize = sizeof(MONITORINFO);

		hMonitor = m_aptDDAdapterInfo[i].hMonitor;
		GetMonitorInfo( hMonitor, &mi );
		
		INT	x = _rcScreen.left;
		INT	y = _rcScreen.top;
		if( (mi.rcMonitor.left <= x && x <= mi.rcMonitor.right) &&
			(mi.rcMonitor.top  <= y && y <= mi.rcMonitor.bottom)   ) {
			nNo = i;
			break;
		}
	}

	return nNo;
}

RECT CD3d::m_AdjustPositionByAdapterNo( RECT const _rcScreen, INT _nNo )
{
	RECT	rcRet = {0,};

	rcRet = _rcScreen;
	do 
	{
		if( 1 == m_nDDAdapterCount ) {
			break;
		}

		if( -1 == _nNo ) {
			break;
		}

		HMONITOR hMonitor;
		MONITORINFO mi = {0,};
		mi.cbSize = sizeof(MONITORINFO);
		hMonitor = m_aptDDAdapterInfo[_nNo].hMonitor;
		GetMonitorInfo( hMonitor, &mi );

		rcRet.left  = _rcScreen.left - mi.rcMonitor.left;
		rcRet.right = _rcScreen.right - mi.rcMonitor.left;

		rcRet.top    = _rcScreen.top  - mi.rcMonitor.top;
		rcRet.bottom = _rcScreen.bottom - mi.rcMonitor.top;

	}while(0);

	return rcRet;
}

BOOL CD3d::m_PrimaryPresentation( RECT* const _prcScreen, RECT* const _prc3DCanvas )
{
	/*
	여기 이단계에서도 ScaleUpDown이 발생한다.
	*/
	RECT	rcScrn;
	HWND	hwnd;
	m_pddClipper->GetHWnd( &hwnd );

	if( NULL == _prcScreen ){
		// Clipping Window 전체에 뿌린다고 생각한다.
		GetClientRect( hwnd, &rcScrn );		
	}else{
		SetRect(&rcScrn, _prcScreen->left, _prcScreen->top, _prcScreen->right, _prcScreen->bottom);
	}

	ClientToScreen( hwnd, (POINT*)&rcScrn.left );
	ClientToScreen( hwnd, (POINT*)&rcScrn.right );

	/*
	여기서 멀티모니터(멀티ADAPTER)에 관련된 고려사항이 걸린다.
	ADAPTER 0을 사용하는 경우 두개 모니터에서 나오지만 secondary쪽은 깨져서 나온다.
    ADAPTER 1,2을 사용하는 경우 해당 모니터에서만 나온다.

    MONITOR 좌표계도 고려 대상이다.
	*/
	INT nAdapterNo = m_GetAdapterNoByPosition( rcScrn );
	//ErrDbg(1, "nAdapterNo %d \r\n", nAdapterNo);
	if( -1 == nAdapterNo ) return TRUE;

#ifdef _VT
	if ( ( m_nUseDDAdapterNo != nAdapterNo )  || ( m_bFontChanged ) )
	{
		m_bFontChanged = FALSE;
#else
	if( m_nUseDDAdapterNo != nAdapterNo )	{
#endif // _VT

		m_Endup();

		//rickie 2008.08.13
/*		if( NULL != _prcScreen )
			m_iUserFontSize = (_prcScreen->bottom * _D3D_DEFAULT_FONT_SIZE) / 1024;
		else
*/			m_iUserFontSize = (_prc3DCanvas->bottom * _D3D_DEFAULT_FONT_SIZE) / 1024;
//		TRACE("Font Size %d\n", m_iUserFontSize);
		m_Setup( nAdapterNo, m_eD3dTextureCF, m_hwndClipper, m_dw3DCanvasWidth, m_dw3DCanvasHeight, m_dwTextureWidth, m_dwTextureHeight );

		m_SetDDClipperWindow( m_hwndClipper );
	}
	rcScrn = m_AdjustPositionByAdapterNo( rcScrn, nAdapterNo );
	//ErrDbg(1, "S(%04d,%04d) \r\n", rcScrn.left, rcScrn.top);

	HRESULT	hr =
//	m_pddsPrimary->Blt( (_prcScreen == NULL) ? &rcScrn : _prcScreen,	// DestRect
	m_pddsPrimary->Blt( &rcScrn,	// DestRect
						m_pddsBackbuffer,	// SrcSurface (3DCanvas)
						_prc3DCanvas,		// SrcRect
						DDBLT_ASYNC | DDBLT_DONOTWAIT,
						NULL );

	if( FAILED(hr) ) {
		_MACRO_DDERROR( hr, "Blt" );
		if( hr == DDERR_SURFACELOST ) {
			hr = m_pddsBackbuffer->Restore();

#ifdef _VT
			// Restore all the stuff
			hr = m_pddsPrimary->Restore();

			// Fix up any attached surfaces too
			m_pddsBackbuffer->EnumAttachedSurfaces( NULL, _lpEnumSurfacesCallback3 );
#endif // _VT

		}
		return FALSE;
	}

	return TRUE;
}

BOOL CD3d::m_Clear( RECT tr )
{
	D3DRECT trD3D = { tr.left, tr.top, tr.right, tr.bottom };
	m_pd3dDevice->Clear( 1,
						 &trD3D,
						 D3DCLEAR_TARGET |D3DCLEAR_ZBUFFER,
						 0,
						 1.0f,
						 0 );
	return TRUE;
}

HRESULT	CD3d::_DisplayDDError( HRESULT _hrDDErr, LPTSTR _lpszAPI, LPTSTR _lpszFile, INT _nLineNo )
{
	TCHAR	sz[64] = {0,};

	switch( _hrDDErr )
	{
	case E_NOINTERFACE: wsprintf(sz, "E_NOINTERFACE"); break;
	case DDERR_INVALIDRECT: wsprintf(sz, "DDERR_INVALIDRECT"); break;
	case DDERR_NOBLTHW: wsprintf(sz, "DDERR_NOBLTHW"); break;
	case DDERR_NOCLIPLIST: wsprintf(sz, "DDERR_NOCLIPLIST"); break;
	case DDERR_NODDROPSHW: wsprintf(sz, "DDERR_NODDROPSHW"); break;
	case DDERR_NOMIRRORHW: wsprintf(sz, "DDERR_NOMIRRORHW"); break;
	case DDERR_NORASTEROPHW: wsprintf(sz, "DDERR_NORASTEROPHW"); break;
	case DDERR_NOROTATIONHW: wsprintf(sz, "DDERR_NOROTATIONHW"); break;
	case DDERR_NOSTRETCHHW: wsprintf(sz, "DDERR_NOSTRETCHHW"); break;
	case DDERR_CANNOTATTACHSURFACE: wsprintf(sz, "DDERR_CANNOTATTACHSURFACE"); break;
	case DDERR_SURFACEALREADYATTACHED: wsprintf(sz, "DDERR_SURFACEALREADYATTACHED"); break;
	case DDERR_INVALIDCAPS: wsprintf(sz, "DDERR_INVALIDCAPS"); break;
	case DDERR_INVALIDPIXELFORMAT: wsprintf(sz, "DDERR_INVALIDPIXELFORMAT"); break;
	case DDERR_NOALPHAHW: wsprintf(sz, "DDERR_NOALPHAHW"); break;
	case DDERR_NOEMULATION: wsprintf(sz, "DDERR_NOEMULATION"); break;
	case DDERR_NOFLIPHW: wsprintf(sz, "DDERR_NOFLIPHW"); break;
	case DDERR_NOMIPMAPHW: wsprintf(sz, "DDERR_NOMIPMAPHW"); break;
	case DDERR_NOOVERLAYHW: wsprintf(sz, "DDERR_NOOVERLAYHW"); break;
	case DDERR_NOZBUFFERHW: wsprintf(sz, "DDERR_NOZBUFFERHW"); break;
	case DDERR_OUTOFVIDEOMEMORY: wsprintf(sz, "DDERR_OUTOFVIDEOMEMORY"); break;
	case DDERR_PRIMARYSURFACEALREADYEXISTS: wsprintf(sz, "DDERR_PRIMARYSURFACEALREADYEXISTS"); break;
	case DDERR_UNSUPPORTEDMODE: wsprintf(sz, "DDERR_UNSUPPORTEDMODE"); break;
	case DDERR_SURFACEBUSY: wsprintf(sz, "DDERR_SURFACEBUSY"); break;
	case DDERR_SURFACELOST: wsprintf(sz, "DDERR_SURFACELOST"); break;
	case DDERR_WASSTILLDRAWING: wsprintf(sz, "DDERR_WASSTILLDRAWING"); break;
	case DDERR_IMPLICITLYCREATED: wsprintf(sz, "DDERR_IMPLICITLYCREATED"); break;
	case DDERR_INCOMPATIBLEPRIMARY: wsprintf(sz, "DDERR_INCOMPATIBLEPRIMARY"); break;
	case DDERR_NOEXCLUSIVEMODE: wsprintf(sz, "DDERR_NOEXCLUSIVEMODE"); break;
	case DDERR_UNSUPPORTED: wsprintf(sz, "DDERR_UNSUPPORTED"); break;
	case DDERR_WRONGMODE: wsprintf(sz, "DDERR_WRONGMODE"); break;
	case DDERR_INVALIDCLIPLIST: wsprintf(sz, "DDERR_INVALIDCLIPLIST"); break;
	case DDERR_INVALIDSURFACETYPE: wsprintf(sz, "DDERR_INVALIDSURFACETYPE"); break;
	case DDERR_NOCLIPPERATTACHED: wsprintf(sz, "DDERR_NOCLIPPERATTACHED"); break;
	case DDERR_NOCOOPERATIVELEVELSET: wsprintf(sz, "DDERR_NOCOOPERATIVELEVELSET"); break;
	case DDERR_EXCLUSIVEMODEALREADYSET: wsprintf(sz, "DDERR_EXCLUSIVEMODEALREADYSET"); break;
	case DDERR_HWNDALREADYSET: wsprintf(sz, "DDERR_HWNDALREADYSET"); break;
	case DDERR_HWNDSUBCLASSED: wsprintf(sz, "DDERR_HWNDSUBCLASSED"); break;
	case DDERR_INVALIDOBJECT: wsprintf(sz, "DDERR_INVALIDOBJECT"); break;
	case DDERR_DIRECTDRAWALREADYCREATED: wsprintf(sz, "DDERR_DIRECTDRAWALREADYCREATED"); break;
	case DDERR_GENERIC: wsprintf(sz, "DDERR_GENERIC"); break;
	case DDERR_INVALIDDIRECTDRAWGUID: wsprintf(sz, "DDERR_INVALIDDIRECTDRAWGUID"); break;
	case DDERR_INVALIDPARAMS: wsprintf(sz, "DDERR_INVALIDPARAMS"); break;
	case DDERR_NODIRECTDRAWHW: wsprintf(sz, "DDERR_NODIRECTDRAWHW"); break;
	case DDERR_OUTOFMEMORY: wsprintf(sz, "DDERR_OUTOFMEMORY"); break;
	default: wsprintf(sz, "Unknown"); break;
	}

	//ErrDbg(1, "%s - %s(%d) at %s(%d line) \r\n", _lpszAPI, sz, LOWORD(_hrDDErr), _lpszFile, _nLineNo );
	return 0;
}

#ifdef _VT

BOOL CD3d::m_DrawCircle( RECT _rc3DCanvas, SIZE _sizeSrcImage, POINT _ptCentre, double _fRadius, BOOL _bFill, BYTE _cbAlpha, COLORREF	_clrCircle )
{
	DWORD	dwColor = 0;
	dwColor = (DWORD)_clrCircle;
	dwColor |= (DWORD)(_cbAlpha << 24);

	tDRAWRECORD	atDrawRecords[1];
	ZeroMemory( atDrawRecords, sizeof(atDrawRecords) );

	atDrawRecords[0].drawType	= DRT_CIRCLE;
	atDrawRecords[0].colorValue	= dwColor;
	atDrawRecords[0].srcSize 	= _sizeSrcImage;

	// Union
	atDrawRecords[0].circle.ptCentre	= _ptCentre;
	atDrawRecords[0].circle.fRadius		= _fRadius;
	atDrawRecords[0].circle.bFill		= _bFill;

#ifdef _DRAWRECORDS_ALL
	_PushRecord( _rc3DCanvas, atDrawRecords );
#else
	_DrawRecords( _rc3DCanvas, 0.9f, 1, atDrawRecords );
#endif

	return TRUE;
}

//______________________________________________________
//	
//	This function is used to draw a circular arc between
//	the two small arrows for zone stuff.
//______________________________________________________
//	
BOOL CD3d::m_DrawCircularArc( RECT _rc3DCanvas, 
							  SIZE _sizeSrcImage, 
							  POINT _ptCentre, 
							  double _fRadius, 
							  unsigned short _usWidth, 
							  double _startRadius,
							  double _finishRadius,
							  BYTE _cbAlpha, 
							  COLORREF _clrCircle )
{
	DWORD	dwColor = 0;
	dwColor = (DWORD)_clrCircle;
	dwColor |= (DWORD)(_cbAlpha << 24);

	tDRAWRECORD	atDrawRecords[1];
	ZeroMemory( atDrawRecords, sizeof(atDrawRecords) );

	atDrawRecords[0].drawType	= DRT_CIRCULARARC;
	atDrawRecords[0].colorValue	= dwColor;
	atDrawRecords[0].srcSize 	= _sizeSrcImage;

	// Union
	atDrawRecords[0].circularArc.ptCentre	= _ptCentre;
	atDrawRecords[0].circularArc.fRadius	= _fRadius;
	atDrawRecords[0].circularArc.usWidth	= _usWidth;
	atDrawRecords[0].circularArc.dStartRadius	= _startRadius;
	atDrawRecords[0].circularArc.dFinishRadius	= _finishRadius;

#ifdef _DRAWRECORDS_ALL
	_PushRecord( _rc3DCanvas, atDrawRecords );
#else
	_DrawRecords( _rc3DCanvas, 0.9f, 1, atDrawRecords );
#endif

	return TRUE;
}

ULONG CD3d::_Triangulate( RECT _rc3DCanvas, tDRAWRECORD*	_aptDrawRecords )
{
	int i, j;
	int tcount;
	Vector2dVector polygonpt, triangleresult;

	MYPOLYGON* aptPoly = _aptDrawRecords->polygons.aptPolygon;

	int trW = _rc3DCanvas.right  - _rc3DCanvas.left;
	int trH = _rc3DCanvas.bottom - _rc3DCanvas.top;
	int srW = _aptDrawRecords->srcSize.cx;
	int srH = _aptDrawRecords->srcSize.cy;
	ULONG n = 0;

	for ( i = 0; i <_aptDrawRecords->polygons.nPolygon; i ++ )
	{
		if ( aptPoly->cnt_point > 3 )
		{
			for ( j = 0; j < aptPoly->cnt_point; j++ )
				polygonpt.push_back( Vector2d( aptPoly->atpoint[j].x,  aptPoly->atpoint[j].y ) );

			if ( !Triangulate::Process( polygonpt, triangleresult ) )
			{
	//			AfxMessageBox(_T("Failed Triangulation"));
			}

			tcount = triangleresult.size()/3;  
		}
		else if ( aptPoly->cnt_point == 3 )
		{
			for ( j = 0; j < 3; j++ )
				triangleresult.push_back( Vector2d( aptPoly->atpoint[j].x,  aptPoly->atpoint[j].y ) );

			tcount = 1;
		}
		else
			return FALSE;

		for ( j = 0; j < tcount ; j++ )
		{

			const Vector2d &p0 = triangleresult[j*3+0];
			const Vector2d &p1 = triangleresult[j*3+1];
			const Vector2d &p2 = triangleresult[j*3+2];

			POINT pt0, pt1, pt2;
			pt0.x = p0.GetX();
			pt0.y = p0.GetY();

			pt1.x = p1.GetX();
			pt1.y = p1.GetY();

			pt2.x = p2.GetX();
			pt2.y = p2.GetY();

			pt0.x = max( 0, pt0.x );
			pt0.x = min( _aptDrawRecords->srcSize.cx, pt0.x );
			pt0.y = max( 0, pt0.y );
			pt0.y = min( _aptDrawRecords->srcSize.cy, pt0.y );

			pt1.x = max( 0, pt1.x );
			pt1.x = min( _aptDrawRecords->srcSize.cx, pt1.x );
			pt1.y = max( 0, pt1.y );
			pt1.y = min( _aptDrawRecords->srcSize.cy, pt1.y );

			pt2.x = max( 0, pt2.x );
			pt2.x = min( _aptDrawRecords->srcSize.cx, pt2.x );
			pt2.y = max( 0, pt2.y );
			pt2.y = min( _aptDrawRecords->srcSize.cy, pt2.y );

			// Triangle Vertex를 만든다.
			// 뿌릴 사각형을 삼각형 여러개로 표현한다.

			m_aptRectAndLineTLV[n+0].dvSX = (float)(pt0.x * trW/srW + _rc3DCanvas.left);
			m_aptRectAndLineTLV[n+0].dvSY = (float)(pt0.y * trH/srH + _rc3DCanvas.top);
			m_aptRectAndLineTLV[n+1].dvSX = (float)(pt1.x * trW/srW + _rc3DCanvas.left);
			m_aptRectAndLineTLV[n+1].dvSY = (float)(pt1.y * trH/srH + _rc3DCanvas.top);
			m_aptRectAndLineTLV[n+2].dvSX = (float)(pt2.x * trW/srW + _rc3DCanvas.left);
			m_aptRectAndLineTLV[n+2].dvSY = (float)(pt2.y * trH/srH + _rc3DCanvas.top);
			m_aptRectAndLineTLV[n+3].dvSX = (float)(pt0.x * trW/srW + _rc3DCanvas.left);
			m_aptRectAndLineTLV[n+3].dvSY = (float)(pt0.y * trH/srH + _rc3DCanvas.top);

			n += 4;
		}
		aptPoly++;
	}
	return n;
}

//_______________________________________________________________
//	Get the size of the Client Window (IE)
HRESULT CD3d::m_GetClientWindowSize( unsigned int *p_uiClientWidth, unsigned int *p_uiClientHeight )
{
	RECT	rcScrn;
	HWND	hwnd;
	HRESULT result;

	if ( m_pddClipper )
	{
		m_pddClipper->GetHWnd( &hwnd );
		result = GetClientRect( hwnd, &rcScrn );
	
		*p_uiClientWidth = rcScrn.right - rcScrn.left;
		*p_uiClientHeight = rcScrn.bottom - rcScrn.top;
	}
	else
	{
		*p_uiClientWidth = 0;
		*p_uiClientHeight = 0;
	}
#ifdef _VMR_EXPORT
	*p_uiClientWidth	= m_rcCanvas.right - m_rcCanvas.left;
	*p_uiClientHeight	= m_rcCanvas.bottom - m_rcCanvas.top;
#endif
	return result;
}

void CD3d::m_SetFontSize( int iFontSize )
{
	if ( m_iUserFontSize != iFontSize )
	{
		m_bFontChanged = TRUE;
		m_iUserFontSize = iFontSize;
	}
}


BOOL CD3d::_UpdateBrushAlpha( eBRUSHTYPE brushType, BYTE _cbAlpha )
{
	// Lock the surface and transfer the bits...
	unsigned char *pBytes;

	DWORD dwPitch;
	if( _GetSurfacePointer( m_pddsBrushTextures[brushType], NULL, &pBytes, &dwPitch ) )
	{
		unsigned char *pLine = pBytes;
		for( int y = 0; y < 16; y++, pLine += dwPitch )
		{
			unsigned int *pWord = (unsigned int *)pLine;

			// Modify the alpha slightly coz the textures don't show up so well
			unsigned int uiAlpha = _cbAlpha;

			uiAlpha = min( uiAlpha + 64, 255 );

			uiAlpha <<= 24;

			for( int x = 0; x < 16; x++, pWord++ )
			{
				// Simple test - if byte 0 is != 0 then we change the alpha
				// otherwise we assume it's totally transparent
				if( *pWord & 0x00FFFFFF )
				{
					*pWord = (*pWord & 0x00FFFFFF) + uiAlpha;
				}
			}
		}

		_ReleaseSurfacePointer( m_pddsBrushTextures[brushType] );
		
	}
	else
	{
		ASSERT( FALSE );
	}

	return TRUE;
}

BOOL CD3d::_DrawPolygonVCA( RECT			_rc3DCanvas,
							D3DVALUE		_foZValue,
							ULONG			_nRecordCount,
							tDRAWRECORD		*_aptDrawRecords )
{
	int n, i;

	n = _Triangulate( _rc3DCanvas, _aptDrawRecords );
	_SetVertexColorValue( m_aptRectAndLineTLV, 0, n, _aptDrawRecords->colorValue );
	_SetVertexZValue( m_aptRectAndLineTLV, 0, n, _foZValue );
	_EnableAlpha();

	// See if we have to apply a texture
	if( BRT_SOLID != _aptDrawRecords->brushType )
	{
		ASSERT( _aptDrawRecords->brushType < MAXCOUNT_BRUSHTEXTURES );	// Overflow!

		_UpdateBrushAlpha( _aptDrawRecords->brushType, _aptDrawRecords->colorValue >> 24 );

		m_pd3dDevice->SetTexture( 0, m_pddsBrushTextures[ _aptDrawRecords->brushType ] );

		// Add offset for correct texel-pixel mapping
		for( i = 0; i < n; i++ )
		{
			_D3DTLVERTEX *pVtx = &m_aptRectAndLineTLV[i];

			_SetVertexTextureXY( pVtx, 0, 1, pVtx->dvSX/BRUSH_TEXTURESIZE, pVtx->dvSY/BRUSH_TEXTURESIZE );

			pVtx->dvSX -= 0.5f;
			pVtx->dvSY -= 0.5f;
		}
	}

	// Render the polygon filling
	for ( i = 0; i < n >> 2; i ++ )
	{
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, &m_aptRectAndLineTLV[i<<2], 4, NULL );
	}

	m_pd3dDevice->SetTexture( 0, NULL );


	// BW - if we drew with a brush, then draw a line without texture round the edge to box it all in...
	if( BRT_SOLID != _aptDrawRecords->brushType )
	{
		MYPOLYGON *pPoly = _aptDrawRecords->polygons.aptPolygon;

		int l = 0;
		for( i = 0; i < _aptDrawRecords->polygons.nPolygon; i++, pPoly++ )
		{
			// Rescale the original co-ordinates into line segments
			
			int trW = _rc3DCanvas.right  - _rc3DCanvas.left;
			int trH = _rc3DCanvas.bottom - _rc3DCanvas.top;
			int srW = _aptDrawRecords->srcSize.cx;
			int srH = _aptDrawRecords->srcSize.cy;

			for( int j = 0; j < pPoly->cnt_point; j++ )
			{
				m_aptRectAndLineTLV[l].dvSX = (float)(pPoly->atpoint[j].x * trW/srW + _rc3DCanvas.left);
				m_aptRectAndLineTLV[l].dvSY = (float)(pPoly->atpoint[j].y * trH/srH + _rc3DCanvas.top);
				l++;
			}

			// Connect the last segment
			m_aptRectAndLineTLV[l].dvSX = m_aptRectAndLineTLV[0].dvSX;
			m_aptRectAndLineTLV[l].dvSY = m_aptRectAndLineTLV[0].dvSY;
			l++;

		}

		m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, D3DFVF_TLVERTEX, m_aptRectAndLineTLV, l, NULL );
	}

	return TRUE;

}



//__________________________________________________________
//
//__________________________________________________________
//

void CD3d::PauseVideo(bool bPaused)
{
	m_bVideoPaused = bPaused;

}

void CD3d::RenderCalibrationGrid()
{
	m_pd3dDevice->BeginScene();
	
	unsigned int w, h;

	DWORD stateHandle;

	m_pd3dDevice->CreateStateBlock( D3DSBT_ALL, &stateHandle); 
 
	/// save the current device state
	m_pd3dDevice->CaptureStateBlock(stateHandle );

	/// clear z buffer so this is draw on top of previous shit
	m_pd3dDevice->Clear( 0, NULL, /*D3DCLEAR_TARGET |*/ D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
                      D3DRGBA(0.0f,0.0f,0.0f,1.0f), 1.0f, 0 );

	m_pd3dDevice->SetTexture( 0, NULL );

//	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_SORTINDEPENDENT);
//	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_EDGEANTIALIAS, TRUE);

	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_STENCILENABLE, FALSE);
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,	TRUE );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND,			D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND,			D3DBLEND_INVSRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE,	FALSE );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF,			0 );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_CULLMODE,			D3DCULL_NONE);
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FILLMODE,			D3DFILL_SOLID);
//	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_NORMALIZENORMALS,	TRUE);	/// need this because scaling screws up normals
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE,			TRUE );

	m_GetClientWindowSize(&w, &h);
	D3DVIEWPORT7 Viewport =  { 0, 0, w, h, 0.0f, 1.0f };
	m_pd3dDevice->SetViewport( &Viewport );

	SetupMatricies();

	m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, (D3DMATRIX *)&m_matProjModel );
	m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_VIEW, (D3DMATRIX *)&m_matView );
	
	DrawGroundPlane();

	if (m_bRulerEnabled)
		DrawRuler();

	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SPECULARENABLE, FALSE );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_AMBIENT, D3DRGB(0.4f,0.4f,0.4f) );
//	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_CULLMODE,			D3DCULL_CW);
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_NORMALIZENORMALS,	TRUE);	/// need this because scaling screws up normals
	m_pd3dDevice->SetLight(0, &m_light);
	m_pd3dDevice->LightEnable(0, TRUE);



	for (int i = m_iNumObjects-1; i >= 0; i--)
	{
		//DrawObject(i);
		DrawPerson(i);
	}

	/// restore previous state
	m_pd3dDevice->ApplyStateBlock(stateHandle);
	
	m_pd3dDevice->DeleteStateBlock(stateHandle); 
	
	/// clear z buffer so other shit after this is drawn on top it
	m_pd3dDevice->Clear( 0, NULL, /*D3DCLEAR_TARGET |*/ D3DCLEAR_ZBUFFER,
                      D3DRGBA(0.0f,0.0f,0.0f,1.0f), 1.0f, 0 );

	m_pd3dDevice->EndScene();
}

HRESULT CD3d::DeInitGridMesh()
{
	_SAFE_RELEASE(m_pSphereMesh);
	_SAFE_RELEASE(m_pCylinderMesh);
	_SAFE_RELEASE(m_pConeMesh);
	_SAFE_RELEASE(m_pddsGroundTexture );

/*	
	for (int n = 0; n < 6; n++)
		_SAFE_RELEASE(m_pddsSkyboxTextures[n] );
*/
//	for (int i = 1; i < m_iNumObjects; i++)
//		delete m_objectModels[i].pFileObject;

	m_iNumObjects = 0;

	return S_OK;
}


void CD3d::SetupMatricies()
{
	D3DXMATRIX matTranslation, matModelRotationTilt, matModelTiltLevel, matModelRotationPan, matModelRotationRoll;

	D3DXMatrixPerspectiveFov( &m_matProjModel, D3DX_PI*m_CalibInfo.fFOV/180.0f, float(m_csVideoSize.cy)/float(m_csVideoSize.cx), 1.0f, 1000.0f );
	D3DXMatrixPerspectiveFov( &m_matProjModel_Clipped, D3DX_PI*m_CalibInfo.fFOV/180.0f, float(m_csVideoSize.cy)/float(m_csVideoSize.cx), 1.0f, 510.0f );

	D3DXMatrixTranslation(&matTranslation, 0, 0, -m_CalibInfo.fHeight);
	D3DXMatrixRotationX(&matModelRotationTilt, m_vecGridRot.x);
	D3DXMatrixRotationX(&matModelTiltLevel, -D3DX_PI/2);
	D3DXMatrixRotationY(&matModelRotationPan, m_vecGridRot.z);
	D3DXMatrixRotationZ(&matModelRotationRoll, m_vecGridRot.y);

	m_matView_noTrans = matModelTiltLevel*matModelRotationPan*matModelRotationTilt*matModelRotationRoll;
	m_matView = matTranslation*m_matView_noTrans;


}


//-----------------------------------------------------------------------------
// Name: CalcFileObjectSizeCB()
// Desc: Callback used to calculate the radius of a sphere that encloses all
//       the meshes in the file.
//-----------------------------------------------------------------------------
BOOL CD3d::CalcFileObjectSizeCB( CD3DFileObject* pObject, D3DMATRIX* pmat,
                           VOID* pContext )
{
    ObjectModel*     pModel = (ObjectModel*)pContext;
    D3DVERTEX* pVertices;
    DWORD      dwNumVertices;

	pModel->fMaxX = -FLT_MAX;
	pModel->fMaxY = -FLT_MAX;
	pModel->fMaxZ = -FLT_MAX;
	pModel->fMinX =  FLT_MAX;
	pModel->fMinY =  FLT_MAX;
	pModel->fMinZ =  FLT_MAX;

    if( SUCCEEDED( pObject->GetMeshGeometry( &pVertices, &dwNumVertices, 
                                             NULL, NULL ) ) )
    {
        for( DWORD i=0; i<dwNumVertices; i++ )
        {
            FLOAT x = pVertices[i].x;
            FLOAT y = pVertices[i].y;
            FLOAT z = pVertices[i].z;

  
            if( x > pModel->fMaxX )
                pModel->fMaxX = x;
			if( x < pModel->fMinX)
				pModel->fMinX = x;
			
			if( y > pModel->fMaxY)
                pModel->fMaxY = y;
			if( y < pModel->fMinY)
				pModel->fMinY = y;

			if( z > pModel->fMaxZ)
                pModel->fMaxZ = z;
			if (z < pModel->fMinZ)
				pModel->fMinZ = z;
        }
    }

    // Keep enumerating file objects
    return FALSE;
}


BOOL CD3d::LoadXFile(LPCSTR strFileName, ObjectModel *pModel)




{
	
    CD3DFile* pFileObject = new CD3DFile();
   
#ifdef _VMR_EXPORT
	if( FAILED( pFileObject->Load((CHAR*) strFileName,  GetModuleHandle( _T( "VCAMetaRender.dll" )) ) ) )
#else
#ifdef _DEBUG
    if( FAILED( pFileObject->Load((CHAR*) strFileName,  GetModuleHandle( _T( "AxVCA_d.ocx")) ) ) )
#else
    if( FAILED( pFileObject->Load((CHAR*) strFileName,  GetModuleHandle( _T( "AxVCA.ocx")) ) ) )
#endif
#endif
    {
        MessageBox( NULL, TEXT("Error loading specified X file"),
                    TEXT("XFile"), MB_OK|MB_ICONERROR );
        return E_FAIL;
    }

    pModel->pFileObject = pFileObject;

	pFileObject->EnumObjects( CalcFileObjectSizeCB, NULL, (VOID*)pModel );

	pModel->fScale = 1.8f/pModel->fMaxY;
	
	pModel->vecRot = D3DXVECTOR3(D3DX_PI/2, 0, -D3DX_PI/2); 

    // Return successful
    return S_OK;

}


BOOL CD3d::InitTextures()
{
	const DWORD bmpsize = 32;
	const DWORD colour = 0x0000FF00; // a lovely shade of green

	LPDIRECTDRAWSURFACE7 pddsTemp;

	D3DXInitialize();
	m_pd3dDevice->GetCaps(&m_d2dDevDesc);

	unsigned char bits[ bmpsize * bmpsize * 3 + 4];

	pddsTemp = _CreateCustomSurface( m_pDD, SURFACETYPE_MANAGEDTEXTURE, &DDPF_RGBA32, bmpsize, bmpsize );
	if( !pddsTemp )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	
	// Load the bitmap
#ifdef _VMR_EXPORT
	HBITMAP hBmp = LoadBitmap( GetModuleHandle( _T("VCAMetaRender.dll"))/*theApp.hInst*/, MAKEINTRESOURCE( IDB_GROUND ) );
#else
#ifdef _DEBUG
	HBITMAP hBmp = LoadBitmap( GetModuleHandle( _T( "AxVCA_d.ocx"))/*theApp.hInst*/, MAKEINTRESOURCE( IDB_GROUND ) );
#else
	HBITMAP hBmp = LoadBitmap( GetModuleHandle( _T( "AxVCA.ocx"))/*theApp.hInst*/, MAKEINTRESOURCE( IDB_GROUND ) );
#endif
#endif
	ASSERT( hBmp );
	if( hBmp == NULL)
	{
		TCHAR buf[100];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, buf, 100, 0);
		MessageBox( NULL, buf/*TEXT("Error loading ground texture")*/,
                    TEXT("Load"), MB_OK|MB_ICONERROR );
        return FALSE;
	}

	BITMAPINFO bmi;
	memset( &bmi, 0, sizeof( BITMAPINFO ) );
	bmi.bmiHeader.biSize		= sizeof( BITMAPINFOHEADER );
	bmi.bmiHeader.biWidth		= bmpsize;
	bmi.bmiHeader.biHeight		= bmpsize;
	bmi.bmiHeader.biPlanes		= 1;
	bmi.bmiHeader.biCompression	= BI_RGB;
	bmi.bmiHeader.biBitCount	= 24;

	HDC hDC = ::GetDC( AfxGetMainWnd()->GetSafeHwnd() );
	::GetDIBits( hDC, hBmp, 0, bmpsize, bits, &bmi, DIB_RGB_COLORS );
	::ReleaseDC( NULL, hDC );

	DeleteObject(hBmp);

	// Lock the surface and transfer the bits...
	unsigned char *pBytes;
	unsigned char ucAlpha = 0xF0;
	DWORD dwPitch;
	if( _GetSurfacePointer( pddsTemp, NULL, &pBytes, &dwPitch ) )
	{
		unsigned char *pLine = pBytes;
		for( int y = 0; y < bmpsize; y++, pLine += dwPitch )
		{
			unsigned char *pByte = pLine;
			for( int x = 0; x < bmpsize; x++ )
			{
				int offset = ((y*bmpsize) + x) * 3;
				*(DWORD*)pByte = colour;
				pByte += 3;
				*pByte++ = bits[offset]; // alpha
			}
		}
		
		_ReleaseSurfacePointer( pddsTemp );
		
	}
	else
	{
		ASSERT( FALSE );
	}

	DeleteObject(hBmp);

	DWORD width = bmpsize;
	DWORD height = bmpsize;
	D3DX_SURFACEFORMAT fmt = D3DX_SF_A8R8G8B8;


	VERIFY(SUCCEEDED (D3DXCreateTexture(m_pd3dDevice, NULL, &width, &height, &fmt, NULL, &m_pddsGroundTexture,  NULL)));

	VERIFY(SUCCEEDED( D3DXLoadTextureFromSurface(m_pd3dDevice, m_pddsGroundTexture, D3DX_DEFAULT, pddsTemp, NULL, NULL, D3DX_FT_DEFAULT)));

	pddsTemp->Release();

/*
	///// skybox textures
	LoadTextureFromResource(&m_pddsSkyboxTextures[0], IDB_SKY_NEGX, 512);
	LoadTextureFromResource(&m_pddsSkyboxTextures[1], IDB_SKY_POSX, 512);
	LoadTextureFromResource(&m_pddsSkyboxTextures[2], IDB_SKY_NEGY, 512);
	LoadTextureFromResource(&m_pddsSkyboxTextures[3], IDB_SKY_POSY, 512);
	LoadTextureFromResource(&m_pddsSkyboxTextures[4], IDB_SKY_NEGZ, 512);
	LoadTextureFromResource(&m_pddsSkyboxTextures[5], IDB_SKY_POSZ, 512);
*/
		///// ruler texture
	LoadTextureFromResource(&m_pddsRulerTexture, IDB_RULER, 16);



	return TRUE;



}

BOOL CD3d::LoadTextureFromResource(LPDIRECTDRAWSURFACE7 *ppddsTexture, DWORD idRes, DWORD bmpsize)
{

	unsigned char *bits = new unsigned char[ bmpsize * bmpsize * 3 + 4];

	D3DX_SURFACEFORMAT fmt = D3DX_SF_R8G8B8; 

		// Load the bitmap
#ifdef _VMR_EXPORT
	HBITMAP hBmp = LoadBitmap( GetModuleHandle( _T("VCAMetaRender.dll")), MAKEINTRESOURCE( idRes ) );
#else
#ifdef _DEBUG
	HBITMAP hBmp = LoadBitmap( GetModuleHandle( _T( "AxVCA_d.ocx")), MAKEINTRESOURCE( idRes ) );
#else
	HBITMAP hBmp = LoadBitmap( GetModuleHandle( _T( "AxVCA.ocx")), MAKEINTRESOURCE( idRes ) );
#endif
#endif
	ASSERT( hBmp );

	BITMAPINFO bmi;
	memset( &bmi, 0, sizeof( BITMAPINFO ) );
	bmi.bmiHeader.biSize		= sizeof( BITMAPINFOHEADER );
	bmi.bmiHeader.biWidth		= bmpsize;
	bmi.bmiHeader.biHeight		= bmpsize;
	bmi.bmiHeader.biPlanes		= 1;
	bmi.bmiHeader.biCompression	= BI_RGB;
	bmi.bmiHeader.biBitCount	= 24;

	HDC hDC = ::GetDC( AfxGetMainWnd()->GetSafeHwnd() );
	::GetDIBits( hDC, hBmp, 0, bmpsize, bits, &bmi, DIB_RGB_COLORS );
	::ReleaseDC( NULL, hDC );

	DeleteObject(hBmp);


	if (!SUCCEEDED (D3DXCreateTexture(m_pd3dDevice, NULL, &bmpsize, &bmpsize, &fmt, NULL, ppddsTexture,  NULL)))
	{
		delete [] bits;
		return FALSE;
	}

	fmt = D3DX_SF_R8G8B8;
	if(!SUCCEEDED( D3DXLoadTextureFromMemory(m_pd3dDevice, *ppddsTexture, D3DX_DEFAULT, bits, NULL, fmt, bmpsize*3, NULL, D3DX_FT_DEFAULT )))
	{
		delete [] bits;
		return FALSE;
	}

	delete [] bits;
	return TRUE;
}

	/// different colour materials
D3DCOLORVALUE CD3d::m_colourLUT[NUM_COLOURS] = { 
	{0.2f, 0.2f, 0.8f, 1.0f },
	{0.8f, 0.2f, 0.2f, 1.0f },
	{0.2f, 0.8f, 0.2f, 1.0f },
	{0.2f, 0.8f, 0.8f, 1.0f },
	{0.8f, 0.8f, 0.2f, 1.0f },
	{0.8f, 0.2f, 0.8f, 1.0f }
}; 

HRESULT CD3d::InitGridMesh()
{
	memset(&m_CalibInfo, 0, sizeof(m_CalibInfo));

	m_CalibInfo.fHeight = 10.0;
	m_CalibInfo.fFOV	= 40.0;
	m_CalibInfo.fTilt	= 30.0;
	m_CalibInfo.fPan	= 0.0;
	m_CalibInfo.fRoll	= 0.0;

	m_fHumanHeight = 1.8f;
	
	SetupMatricies();

	m_bParamsSet = false;
	m_bTiltingGrid = false;
	m_bSkyEnabled = false;

	m_vecGridRot = D3DXVECTOR3(D3DXToRadian(m_CalibInfo.fTilt), 0.0f, D3DXToRadian(m_CalibInfo.fPan));
	

	m_bRulerEnabled = false;
	m_calib_callback_fxn = NULL;

	ZeroMemory(m_objectModels, sizeof(m_objectModels));

	m_iNumObjects = 0;
//	LoadXFile(MAKEINTRESOURCE(IDR_XFILEMALE), &m_objectModels[0]);
//	LoadXFile(MAKEINTRESOURCE(IDR_XFILEFEMALE), &m_objectModels[1]);

	_insertObjectModel(D3DXVECTOR3(0, 0, 0));
	_insertObjectModel(D3DXVECTOR3(0, 0, 0));


	
	///// sphere
	if (S_OK != D3DXCreateSphere(m_pd3dDevice, 0.1f, 10, 10, 0, &m_pSphereMesh)) return S_FALSE;

	///// cylinder
	if (S_OK != D3DXCreateCylinder(m_pd3dDevice, 0.1f, 0.1f, 1.0f, 10, 5, 0, &m_pCylinderMesh)) return S_FALSE;

	/// cone
	if (S_OK != D3DXCreateCylinder(m_pd3dDevice, 0.15f, 0.25f, 1.0f, 10, 5, 0, &m_pConeMesh)) return S_FALSE;
		

	//// light
	D3DXVECTOR4 lightDir(-0.5f, 0.5f, -1.0f, 0.0f);

	ZeroMemory(&m_light, sizeof(D3DLIGHT7));
	m_light.dltType = D3DLIGHT_DIRECTIONAL;
	m_light.dcvDiffuse.r = m_light.dcvSpecular.r = 1.0f;
	m_light.dcvDiffuse.g = m_light.dcvSpecular.g = 1.0f;
	m_light.dcvDiffuse.b = m_light.dcvSpecular.b = 1.0f;
	m_light.dvPosition.x = 0.0f;
	m_light.dvPosition.y = 0.0f;
	m_light.dvPosition.z = 0.0f;
	m_light.dvDirection.x = lightDir.x;
	m_light.dvDirection.y = lightDir.y;
	m_light.dvDirection.z = lightDir.z;

	// Don't attenuate.
	m_light.dvAttenuation0 = 1.0f; 
	m_light.dvRange        = D3DLIGHT_RANGE_MAX;

	/// shadow stuff
	D3DXPLANE	groundPlane(0.0f, 0.0f, -1.0f, 0.0f);
	D3DXMatrixShadow(&m_matShadow, &lightDir, &groundPlane);	
	memset(&m_materialShadow, 0, sizeof(D3DMATERIAL7));
	m_materialShadow.diffuse.a = 0.5f;
	m_materialShadow.ambient.a = 0.5f;
	
	/// verticies for ground and skybox
	m_vGroundVerticies[0] = D3DVERTEX(D3DVECTOR(-10.0f,  10.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, 1.0f), -5.0f,	5.0f);
	m_vGroundVerticies[1] = D3DVERTEX(D3DVECTOR( 10.0f,  10.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, 1.0f),  5.0f,	5.0f);
	m_vGroundVerticies[2] = D3DVERTEX(D3DVECTOR(-10.0f, -10.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, 1.0f), -5.0f, -5.0f);
	m_vGroundVerticies[3] = D3DVERTEX(D3DVECTOR( 10.0f, -10.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, 1.0f),  5.0f, -5.0f);
/*
	float dimension = 510.0f;
	for (int n = 0; n < 4; n++)
	{
		int mod1 = (n & 0x1);
		int mod2 = (n & 0x2) >> 1;
		int sign1 = 2*mod1 - 1;
		int sign2 = 2*mod2 - 1;

		m_vSkyboxVerticies[0][n] = D3DVERTEX(D3DVECTOR(-sign1*dimension,  -dimension,  sign2*dimension), D3DVECTOR(0.0f, -1.0f, 0.0f), (float)mod1, (float)mod2);
		m_vSkyboxVerticies[1][n] = D3DVERTEX(D3DVECTOR(sign1*dimension,  dimension,  sign2*dimension), D3DVECTOR(0.0f, 1.0f, 0.0f), (float)mod1, (float)mod2);
		m_vSkyboxVerticies[2][n] = D3DVERTEX(D3DVECTOR(-sign1*dimension,  -sign2*dimension,  -dimension), D3DVECTOR(0.0f, 0.0f, -1.0f), (float)mod1, (float)mod2);
		m_vSkyboxVerticies[3][n] = D3DVERTEX(D3DVECTOR(sign1*dimension,  -sign2*dimension,  dimension), D3DVECTOR(0.0f, 0.0f, 1.0f), (float)mod1, (float)mod2);
		m_vSkyboxVerticies[4][n] = D3DVERTEX(D3DVECTOR(-dimension,  sign1*dimension,  sign2*dimension), D3DVECTOR(-1.0f, 0.0f, 0.0f), (float)mod1, (float)mod2);
		m_vSkyboxVerticies[5][n] = D3DVERTEX(D3DVECTOR(dimension,  -sign1*dimension,  sign2*dimension), D3DVECTOR(1.0f, 0.0f, 0.0f), (float)mod1, (float)mod2);
	}
*/
//	m_vHorizonVerticies[0] =  D3DLVERTEX(D3DVECTOR(-1.0f,  0.0f, -1.0f), 0xFF00FF00, 0, 0.0f,	0.0f);
//	m_vHorizonVerticies[1] =  D3DLVERTEX(D3DVECTOR(1.0f,  0.0f, -1.0f), 0xFF00FF00, 0, 0.0f,	0.0f);

	m_vHorizonVerticies[0] =  D3DLVERTEX(D3DVECTOR(-510.0f,  510.0f, 0.0f), 0xFF00FF00, 0, 0.0f,	0.0f);
	m_vHorizonVerticies[1] =  D3DLVERTEX(D3DVECTOR(510.0f,  510.0f, 0.0f), 0xFF00FF00, 0, 0.0f,		0.0f);
	m_vHorizonVerticies[2] =  D3DLVERTEX(D3DVECTOR(510.0f,  -510.0f, 0.0f), 0xFF00FF00, 0, 0.0f,	0.0f);
	m_vHorizonVerticies[3] =  D3DLVERTEX(D3DVECTOR(-510.0f,  -510.0f, 0.0f), 0xFF00FF00, 0, 0.0f,	0.0f);
	m_vHorizonVerticies[4] =  D3DLVERTEX(D3DVECTOR(-510.0f,  510.0f, 0.0f), 0xFF00FF00, 0, 0.0f,	0.0f);

	InitTextures();
	

	return S_OK;
}

void CD3d::DrawObject(int iModelIdx)
{

	SetupModelMatrix(iModelIdx);

	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &m_objectModels[iModelIdx].matObjectModel);

	m_pd3dDevice->SetTexture( 0, NULL );

	if( m_objectModels[iModelIdx].pFileObject )
            m_objectModels[iModelIdx].pFileObject->Render( m_pd3dDevice, m_objectModels[iModelIdx].bHighlighted );

}

void CD3d::SetupModelMatrix(int iModelIdx)
{
	/*
	D3DXMATRIX matTranslation, matScaling, matRotationX, matRotationZ;

	D3DXMatrixRotationX(&matRotationX, m_objectModels[iModelIdx].vecRot.x);
	D3DXMatrixRotationZ(&matRotationZ, m_objectModels[iModelIdx].vecRot.z);

	D3DXMatrixTranslation(&matTranslation, m_objectModels[iModelIdx].vecPos.x, m_objectModels[iModelIdx].vecPos.y, 0);

	float fScale = m_objectModels[iModelIdx].fScale;

	D3DXMatrixScaling(&matScaling, fScale, fScale, fScale);

	m_objectModels[iModelIdx].matObjectModel = matScaling*matRotationX*matRotationZ*matTranslation;
	*/

	D3DXMATRIX matRotationY, matTranslation, matModelTranslation, matTranslation2;

	D3DXMatrixRotationY(&matRotationY, m_objectModels[iModelIdx].vecRot.z);
	D3DXMatrixTranslation(&matTranslation, 0, 0, -m_fHumanHeight/2);
	D3DXMatrixTranslation(&matTranslation2, 0, 0, m_fHumanHeight/2);

	matRotationY = matTranslation*matRotationY*matTranslation2;

	D3DXMatrixTranslation(&matModelTranslation, m_objectModels[iModelIdx].vecPos.x, m_objectModels[iModelIdx].vecPos.y, 0);

	
	m_objectModels[iModelIdx].matObjectModel = matRotationY*matModelTranslation;

	


}

void CD3d::DrawShadow(ID3DXSimpleShape *pMesh, D3DMATRIX matWorld)
{
	
	DWORD stateHandle;
	D3DXMATRIX matEsp;
	

	m_pd3dDevice->CreateStateBlock( D3DSBT_ALL, &stateHandle); 
 
	/// save the current device state
	m_pd3dDevice->CaptureStateBlock(stateHandle );


	m_pd3dDevice->SetMaterial(&m_materialShadow);

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_STENCILENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_STENCILFUNC, D3DCMP_EQUAL);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_STENCILREF, 0x0);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_STENCILMASK, 0xFFFFFFFF);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_STENCILWRITEMASK, 0xFFFFFFFF);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_STENCILFAIL, D3DSTENCILOP_KEEP);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_STENCILPASS, D3DSTENCILOP_INCR);


	D3DXMatrixTranslation(&matEsp, 0.0f, 0.0f, 0.001f);


	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matWorld*m_matShadow*matEsp));
	pMesh->Draw();
	


		/// restore previous state
	m_pd3dDevice->ApplyStateBlock(stateHandle);
	
	m_pd3dDevice->DeleteStateBlock(stateHandle); 

}

void CD3d::DrawPerson(int iModelIdx)
{
	
	D3DXMATRIX matModelRotation, matTranslation, matScaling, matRotation;
	D3DXMATRIX matLocal, matModel;

	SetupModelMatrix(iModelIdx);

	matModel = m_objectModels[iModelIdx].matObjectModel;

	if ( m_objectModels[iModelIdx].bHighlighted)
	{
		m_materialPerson[iModelIdx].emissive.r = min(0.3f, m_materialPerson[iModelIdx].emissive.r+0.05f);
		m_materialPerson[iModelIdx].emissive.g = min(0.3f, m_materialPerson[iModelIdx].emissive.g+0.05f);
		m_materialPerson[iModelIdx].emissive.b = min(0.3f, m_materialPerson[iModelIdx].emissive.b+0.05f);
	}
	else
	{
		m_materialPerson[iModelIdx].emissive.r = max(0.0f, m_materialPerson[iModelIdx].emissive.r-0.05f);
		m_materialPerson[iModelIdx].emissive.g = max(0.0f, m_materialPerson[iModelIdx].emissive.g-0.05f);
		m_materialPerson[iModelIdx].emissive.b = max(0.0f, m_materialPerson[iModelIdx].emissive.b-0.05f);
	}

	m_pd3dDevice->SetMaterial(&m_materialPerson[iModelIdx]);
	m_pd3dDevice->SetTexture( 0, NULL );

	/// head
	D3DXMatrixScaling(&matScaling, 1.5f, 1.5f, 1.5f); 
	D3DXMatrixTranslation(&matTranslation, 0, 0, m_fHumanHeight-0.125f);
	matLocal = matScaling*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pSphereMesh->Draw();
	DrawShadow(m_pSphereMesh, matLocal*matModel);

	/// feet
	float legangle = D3DXToRadian(8);
	float leglength = 0.85f;
	float feetpos = 0.17f;
	float legwidth = 0.9f;

	D3DXMatrixScaling(&matScaling, legwidth, legwidth, legwidth); 
	D3DXMatrixTranslation(&matTranslation, -feetpos, 0.0f, 0.1f);
	matLocal = matScaling*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pSphereMesh->Draw();
	DrawShadow(m_pSphereMesh, matLocal*matModel);

	D3DXMatrixTranslation(&matTranslation, feetpos, 0.0f, 0.1f);
	matLocal = matScaling*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pSphereMesh->Draw();
	DrawShadow(m_pSphereMesh, matLocal*matModel);

	/// legs
	D3DXMatrixScaling(&matScaling, legwidth, legwidth, leglength); 
	D3DXMatrixTranslation(&matTranslation, feetpos, 0.0f, 0.1f);
	D3DXMatrixRotationY(&matRotation, -legangle);
	matLocal = matScaling*matRotation*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pCylinderMesh->Draw();
	DrawShadow(m_pCylinderMesh, matLocal*matModel);

	D3DXMatrixTranslation(&matTranslation, -feetpos, 0.0f, 0.1f);
	D3DXMatrixRotationY(&matRotation, legangle);
	matLocal = matScaling*matRotation*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pCylinderMesh->Draw();
	DrawShadow(m_pCylinderMesh, matLocal*matModel);

	/// arms & hands
	float armangle = D3DXToRadian(13);
	float armlength = 0.55f;
	float handpos	= 0.3f;
	float handheight = 0.9f;
	float armwidth = 0.6f;

	D3DXMatrixScaling(&matScaling, armwidth, armwidth, armwidth); 
	D3DXMatrixTranslation(&matTranslation, -handpos, 0.0f, handheight);
	matLocal = matScaling*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pSphereMesh->Draw();
	DrawShadow(m_pSphereMesh, matLocal*matModel);

	D3DXMatrixTranslation(&matTranslation, handpos, 0.0f, handheight);
	matLocal = matScaling*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pSphereMesh->Draw();
	DrawShadow(m_pSphereMesh, matLocal*matModel);

	D3DXMatrixScaling(&matScaling, armwidth, armwidth, armlength); 
	D3DXMatrixTranslation(&matTranslation, handpos, 0.0f, handheight);
	D3DXMatrixRotationY(&matRotation, -armangle);
	matLocal = matScaling*matRotation*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pCylinderMesh->Draw();
	DrawShadow(m_pCylinderMesh, matLocal*matModel);

	D3DXMatrixTranslation(&matTranslation, -handpos, 0.0f, handheight);
	D3DXMatrixRotationY(&matRotation, armangle);
	matLocal = matScaling*matRotation*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pCylinderMesh->Draw();
	DrawShadow(m_pCylinderMesh, matLocal*matModel);

	/// body
	D3DXMatrixScaling(&matScaling, 1.0f, 0.5f, 0.65f); 
	D3DXMatrixTranslation(&matTranslation, 0.0f, 0.0f, 0.8f);
	matLocal = matScaling*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pConeMesh->Draw();
	DrawShadow(m_pConeMesh, matLocal*matModel);

	D3DXMatrixScaling(&matScaling, 1.0f, 0.5f, 0.05f); 
	D3DXMatrixTranslation(&matTranslation, 0.0f, 0.0f, 1.5f);
	D3DXMatrixRotationY(&matRotation, D3DX_PI);
	matLocal = matScaling*matRotation*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pConeMesh->Draw();
	DrawShadow(m_pConeMesh, matLocal*matModel);

	// uncomment this for a female version ;)
/*\
	/// titties
	D3DXMatrixScaling(&matScaling, 1.0f, 1.0f, 1.0f); 
	D3DXMatrixTranslation(&matTranslation, -0.1f, -0.1f, 1.3f);
	matLocal = matScaling*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pSphereMesh->Draw();
	DrawShadow(m_pSphereMesh, matLocal*matModel);

	D3DXMatrixScaling(&matScaling, 1.0f, 1.0f, 1.0f);
	D3DXMatrixTranslation(&matTranslation, 0.1f, -0.1f, 1.3f);
	matLocal = matScaling*matTranslation;                                                                           
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &(matLocal*matModel));
	m_pSphereMesh->Draw();
	DrawShadow(m_pSphereMesh, matLocal*matModel);
*/	
	

}

void CD3d::DrawGroundPlane()
{
	D3DXMATRIX matLocal, matRotZ;

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_ANISOTROPIC );
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_MAXANISOTROPY, m_d2dDevDesc.dwMaxAnisotropy); 

	

	if (m_bSkyEnabled)
	{
		/*
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_TEXTUREFACTOR, 0xA0000000  ); // set total alpha for this texture
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR  );
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
		
		/// important! Skybox is always centred on viewer to look correct therefore view matrix is only the rotation (no translation for camera height)
			
		m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, (D3DMATRIX *) &m_matView_noTrans);
		D3DXMatrixIdentity(&matLocal);
		m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &matLocal);
			
		for (int n = 0; n < 6; n++)
		{
			if(m_pddsSkyboxTextures[n])
			{
			

				m_pd3dDevice->SetTexture( 0, m_pddsSkyboxTextures[n] );

			

				m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_VERTEX, m_vSkyboxVerticies[n], 4, 0 );

				
			
			}
		}
		*/

		// draw shit horizon line instead
		
		D3DXMATRIX matRotation, matProj;
		D3DXMatrixRotationX(&matRotation, m_vecGridRot.x);
		D3DXMatrixIdentity(&matLocal);

		// hack of projection to remove x perspective and keep line at maximum z
//		matProj = m_matProjModel; 
//		matProj.m00 = 1.0f;
//		matProj.m22 = 0.0f;
//		matProj.m32 = 0.0f;

		m_pd3dDevice->SetTexture( 0, NULL );

//		m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, (D3DMATRIX *) &matProj);
		m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, (D3DMATRIX *) &m_matView_noTrans);
		m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &matLocal);

		m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, D3DFVF_LVERTEX , m_vHorizonVerticies, 5, 0);

		m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, (D3DMATRIX *) &m_matView);
	

	
		
	}

	if (m_pddsGroundTexture)
	{
		/// set up texture render to mutiply texture alpha with constant TEXTUREFACTOR alpha 
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE  );
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_TEXTUREFACTOR, m_bTransparentGrid?  0x40000000 : 0xFF000000 ); // set total alpha for this texture
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);  
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );
		// wrap texture to repeate grid
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);

		m_pd3dDevice->SetTexture( 0, m_pddsGroundTexture );

		for (int i = -25; i <= 25; i++)
			for (int j = -25; j <= 25; j++)
			{
				D3DXMatrixTranslation(&matLocal, i*20.0f, j*20.0f, 0.0f);
				m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *) &matLocal);
				m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_VERTEX, m_vGroundVerticies, 4, 0 );
			}
		

		// disable alpha multiply
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	}

	

}

void CD3d::EnableCalibDrawSky(BOOL enable)
{
	m_bSkyEnabled = enable;

}

BOOL CD3d::_insertObjectModel(D3DXVECTOR3 pos)
{
	if (m_iNumObjects < MAXNUM_OBJECTS)
	{
		
		m_objectModels[m_iNumObjects].fMinX = -0.35f;
		m_objectModels[m_iNumObjects].fMaxX = 0.35f;
		m_objectModels[m_iNumObjects].fMinY = -0.15f;
		m_objectModels[m_iNumObjects].fMaxY = 0.15f;
		m_objectModels[m_iNumObjects].fMinZ = 0.0f;
		m_objectModels[m_iNumObjects].fMaxZ = m_fHumanHeight;

		memset(&m_materialPerson[m_iNumObjects], 0, sizeof(D3DMATERIAL7));
		m_materialPerson[m_iNumObjects].specular.r = 1.0f;
   		m_materialPerson[m_iNumObjects].specular.g = 1.0f;
		m_materialPerson[m_iNumObjects].specular.b = 1.0f;
		m_materialPerson[m_iNumObjects].specular.a = 0.0f;
		m_materialPerson[m_iNumObjects].power = 40.0f;
		m_materialPerson[m_iNumObjects].diffuse = m_colourLUT[m_iNumObjects%NUM_COLOURS];
		m_materialPerson[m_iNumObjects].ambient = m_colourLUT[m_iNumObjects%NUM_COLOURS];

		m_objectModels[m_iNumObjects].vecPos = pos;
		m_objectModels[m_iNumObjects].vecPos.z = 0;
	

		m_iNumObjects++;
		
		return TRUE;
	}

	return FALSE;

}

void CD3d::InsertObjectModel(CPoint screenCoord)
{
	D3DXVECTOR3 v;
	float fDist2Plane;
	v = UnprojectZplane(screenCoord, -1, m_fHumanHeight/2, &fDist2Plane);

	if (fDist2Plane >= 0.0f && fDist2Plane <= 1.0f)
	{
		
		if(_insertObjectModel(v))
			m_objectModels[m_iNumObjects-1].bHighlighted = true; // max him highlighted
	}

}

void CD3d::TransparentGrid(BOOL enable)
{

	m_bTransparentGrid = enable;
}

float CD3d::GetRulerLength()
{
	if (m_bRulerEnabled)
	{
		float dx = m_vRulerStartPos.x - m_vRulerEndPos.x;
		float dy = m_vRulerStartPos.y - m_vRulerEndPos.y;
		return sqrt(dx*dx + dy*dy);
	}
	else
		return -1.0f;

}

void CD3d::DrawRuler()
{
	D3DVERTEX vRulerVerticies[4];
	D3DXMATRIX matLocal;

	float length = GetRulerLength()*2.5f;

	float x1 = m_vRulerStartPos.x;
	float y1 = m_vRulerStartPos.y;
	float x2 = m_vRulerEndPos.x;
	float y2 = m_vRulerEndPos.y;

	float fTheta = atan2( (float)(y2-y1), (float)(x2-x1) );
	float fx = sin(fTheta);
	float fy = cos(fTheta);
	float fxOff, fyOff;

	fxOff = 0.2f * fx;
	fyOff = 0.2f * fy;

	vRulerVerticies[0] = D3DVERTEX(D3DXVECTOR3(x1 - fxOff, y1 + fyOff, 0.0005f), D3DVECTOR(0.0f, 0.0f, 1.0f), 0.0f, 0.0f);
	vRulerVerticies[1] = D3DVERTEX(D3DXVECTOR3(x2 - fxOff, y2 + fyOff, 0.0005f), D3DVECTOR(0.0f, 0.0f, 1.0f), length, 0.0f);
	vRulerVerticies[2] = D3DVERTEX(D3DXVECTOR3(x1 + fxOff, y1 - fyOff, 0.0005f), D3DVECTOR(0.0f, 0.0f, 1.0f), 0.0f, 1.0f);
	vRulerVerticies[3] = D3DVERTEX(D3DXVECTOR3(x2 + fxOff, y2 - fyOff, 0.0005f), D3DVECTOR(0.0f, 0.0f, 1.0f), length, 1.0f);

	D3DXMatrixIdentity(&matLocal);
	m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, (D3DMATRIX *)&matLocal );
	
//	m_pd3dDevice->SetMaterial(&m_materialPerson[0]);
	m_pd3dDevice->SetTexture( 0, m_pddsRulerTexture );

	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_VERTEX, vRulerVerticies, 4, 0);

	
}

void CD3d::GetCameraParams(VCA_CALIB_INFO_T_D& calibInfo )
{
	calibInfo = m_CalibInfo;
}


void CD3d::SetCameraParams(VCA_CALIB_INFO_T_D& calibInfo, bool bCentre )
{
	
	if (!bCentre)
	{
		HoldModelPositions_Begin();
	}

	m_CalibInfo = calibInfo;

	m_vecGridRot.x = D3DXToRadian(m_CalibInfo.fTilt);
	m_vecGridRot.z = D3DXToRadian(m_CalibInfo.fPan);
	m_vecGridRot.y = D3DXToRadian(m_CalibInfo.fRoll);

	SetupMatricies();
	

	if (bCentre)
	{	
		/// centre the models to nice position
		unsigned int w, h;

		m_GetClientWindowSize(&w, &h);

		CPoint centrePoint;
		if (m_CalibInfo.fTilt >= 0.0f && m_CalibInfo.fTilt <= 60.0f) 
			centrePoint = CPoint(w/2, h-100);
		else if (m_CalibInfo.fTilt > 60.0f && m_CalibInfo.fTilt < 120.0f )
			centrePoint = CPoint(w/2, h/2);
		else if (m_CalibInfo.fTilt < 0.0f)
			centrePoint = CPoint(w/2, h-1);
		else if (m_CalibInfo.fTilt <= 180.0f && m_CalibInfo.fTilt >= 120.0f) 
			centrePoint = CPoint(w/2, 100);
		else if (m_CalibInfo.fTilt > 180.0f)
			centrePoint = CPoint(w/2, 1);
		else
			centrePoint = CPoint(w/2, h/2);

		D3DXVECTOR3 v = UnprojectZplane(centrePoint, -1);

		m_iNumObjects = 2;

		m_objectModels[0].vecPos.x = v.x - 1.0f;
		m_objectModels[0].vecPos.y = v.y - 1.0f;

		m_objectModels[1].vecPos.x = v.x + 1.0f;
		m_objectModels[1].vecPos.y = v.y + 1.0f;
	}
	else // make sure they not on the edge
	{
		HoldModelPositions_End();
//		CheckModelPositions();
	}
}


void CD3d::SetInitialCameraParams(VCA_CALIB_INFO_T_D& calibInfo )
{
	if (m_bParamsSet) return;

	m_bParamsSet = true;

	SetCameraParams(calibInfo, true );


}



void CD3d::PickGrid(CPoint screenCoord)
{
	D3DXVECTOR3 vPickedPos;
	ObjectModel* pObjPicked = PickObject(screenCoord, &vPickedPos);

	if (pObjPicked)
	{
		D3DXVECTOR4 out;
		D3DXVec3Transform(&out, &vPickedPos, &pObjPicked->matObjectModel); // project picked coords from model space to view space
		pObjPicked->vecPickPos = D3DXVECTOR3(out.x, out.y, out.z) - pObjPicked->vecPos; // get offset from current object position
		pObjPicked->bDragging = true;
		m_bTiltingGrid = false;
		
	}
	else
	{
		m_tiltStartPos = screenCoord;
		m_bTiltingGrid = true;
	}
}

ObjectModel* CD3d::PickObject(CPoint screenCoord, D3DXVECTOR3 *pPickedPos)
{
	D3DXVECTOR3 v, vPicked;
	ObjectModel* pObj = m_objectModels;
	ObjectModel* pObjPicked = NULL;
	float fDist2PlaneMin = 1.0f + FLT_EPSILON; //FLT_MAX;
	float fDist2Plane;

	/// try pick object from any bounding box face
	for (int i = 0; i < m_iNumObjects; i++, pObj++)
	{	
		v = UnprojectXplane(screenCoord, i, pObj->fMinX, &fDist2Plane);
		if (v.y > pObj->fMinY && v.y < pObj->fMaxY && v.z > pObj->fMinZ && v.z < pObj->fMaxZ && fDist2Plane < fDist2PlaneMin && fDist2Plane >= 0.0f)
		{
			fDist2PlaneMin = fDist2Plane;
			pObjPicked = pObj;
			vPicked = v;
		}

		v = UnprojectXplane(screenCoord, i, pObj->fMaxX,  &fDist2Plane);
		if (v.y > pObj->fMinY && v.y < pObj->fMaxY && v.z > pObj->fMinZ && v.z < pObj->fMaxZ && fDist2Plane < fDist2PlaneMin  && fDist2Plane >= 0.0f)
		{
			fDist2PlaneMin = fDist2Plane;
			pObjPicked = pObj;
			vPicked = v;
		}
	
		v = UnprojectYplane(screenCoord, i, pObj->fMinY,  &fDist2Plane);
		if (v.x > pObj->fMinX && v.x < pObj->fMaxX && v.z > pObj->fMinZ && v.z < pObj->fMaxZ && fDist2Plane < fDist2PlaneMin  && fDist2Plane >= 0.0f)
		{
			fDist2PlaneMin = fDist2Plane;
			pObjPicked = pObj;
			vPicked = v;
		}

		v = UnprojectYplane(screenCoord, i, pObj->fMaxY,  &fDist2Plane);
		if (v.x > pObj->fMinX && v.x < pObj->fMaxX && v.z > pObj->fMinZ && v.z < pObj->fMaxZ && fDist2Plane < fDist2PlaneMin  && fDist2Plane >= 0.0f)
		{
			fDist2PlaneMin = fDist2Plane;
			pObjPicked = pObj;
			vPicked = v;
		}

		v = UnprojectZplane(screenCoord, i, pObj->fMinZ,  &fDist2Plane);
		if (v.x > pObj->fMinX && v.x < pObj->fMaxX && v.y > pObj->fMinY && v.y < pObj->fMaxY && fDist2Plane < fDist2PlaneMin  && fDist2Plane >= 0.0f)
		{
			fDist2PlaneMin = fDist2Plane;
			pObjPicked = pObj;
			vPicked = v;
		}

		v = UnprojectZplane(screenCoord, i, pObj->fMaxZ,  &fDist2Plane);
		if (v.x > pObj->fMinX && v.x < pObj->fMaxX && v.y > pObj->fMinY && v.y < pObj->fMaxY && fDist2Plane < fDist2PlaneMin  && fDist2Plane >= 0.0f)
		{
			fDist2PlaneMin = fDist2Plane;
			pObjPicked = pObj;
			vPicked = v;
		}
	}
	
	if (pPickedPos)
		*pPickedPos = vPicked;

	return pObjPicked;
	
}

void CD3d::ClipCoords(CPoint &screenCoord)
{
	unsigned int w, h;

	m_GetClientWindowSize(&w, &h);

	screenCoord.x = min(w-1, max(0, screenCoord.x));
	screenCoord.y = min(h-1, max(0, screenCoord.y));


}

void CD3d::DragGrid(CPoint screenCoord)
{

	D3DXVECTOR3 v, v2;
	float fDist2Plane;
	ObjectModel* pObj = m_objectModels;


	for (int i = 0; i < m_iNumObjects; i++, pObj++)
	{
		if (pObj->bDragging)
		{
			ClipCoords(screenCoord);
			v = UnprojectZplane(screenCoord, -1, pObj->vecPickPos.z, &fDist2Plane);

			if (fDist2Plane >= 0.0f && fDist2Plane <= 1.0f)
			{
				pObj->vecPos.y = v.y - pObj->vecPickPos.y;
				pObj->vecPos.x = v.x - pObj->vecPickPos.x;
			}
			break;
		}
	}

	if (m_bTiltingGrid && i == m_iNumObjects )
	{	
	
		unsigned int w, h;

		HoldModelPositions_Begin();

		m_GetClientWindowSize(&w, &h);

		float fang = float(m_tiltStartPos.y - screenCoord.y)*D3DXToRadian(m_CalibInfo.fFOV)/float(h);
		m_vecGridRot.x += fang;
		m_vecGridRot.x = min(D3DXToRadian(90.0f + m_CalibInfo.fFOV/2), max(D3DXToRadian(- m_CalibInfo.fFOV/2), m_vecGridRot.x));


/*//no more panning with mouse :( 
 
		fang = float(m_tiltStartPos.x - screenCoord.x)*D3DXToRadian(m_CalibInfo.fFOV*float(m_csVideoSize.cx)/float(m_csVideoSize.cy))/float(w);
	 
		m_vecGridRot.z += fang;
		// wrap the rotation thru 360deg
		while (m_vecGridRot.z > 2*D3DX_PI)
			m_vecGridRot.z -= 2*D3DX_PI;
		while (m_vecGridRot.z < 0)
			m_vecGridRot.z += 2*D3DX_PI;
*/

		m_tiltStartPos = screenCoord;

		m_CalibInfo.fTilt = D3DXToDegree(m_vecGridRot.x);
		m_CalibInfo.fPan = D3DXToDegree(m_vecGridRot.z);
		
		UpdateCalibInfo();

//		CheckModelPositions();

		HoldModelPositions_End();

	}	

}

void CD3d::MouseOver(CPoint screenCoord)
{


	if (m_bRulerEnabled)
	{
		float fDist2Plane;
		D3DXVECTOR3 v = UnprojectZplane(screenCoord, -1, 0, &fDist2Plane);

		if (fDist2Plane >= 0.0f && fDist2Plane <= 1.0f)
		{
			m_vRulerEndPos = v;
		}
	}

	for (int i = 0; i < m_iNumObjects; i++)
	{
		 m_objectModels[i].bHighlighted = false;
	}


	D3DXVECTOR3 vPickedPos;
	ObjectModel* pObjPicked = PickObject(screenCoord, &vPickedPos);

	if (pObjPicked)
	{
		pObjPicked->bHighlighted = true;
	}
	
	
}

void CD3d::HoldModelPositions_Begin()
{
	D3DXMATRIX matTransform;
	D3DXVECTOR4 out;
	CPoint screenCoord;

	SetupMatricies();


	for (int i = 0; i < m_iNumObjects; i++)
	{
		matTransform = m_matView*m_matProjModel;

		D3DXVec3Transform(&out, &m_objectModels[i].vecPos, &matTransform);

		out.x /= out.w;
		out.y /= out.w;
	
		m_objectModels[i].vecLogicalScreenPos.x = out.x;
		m_objectModels[i].vecLogicalScreenPos.y = out.y;

	}
}



void CD3d::HoldModelPositions_End()
{

	D3DXVECTOR3 v,v2;

	SetupMatricies();
	float fDist2Plane;

	for (int i = 0; i < m_iNumObjects; i++)
	{

		v.x = m_objectModels[i].vecLogicalScreenPos.x;
		v.y = m_objectModels[i].vecLogicalScreenPos.y;
		v.z = 0;
	
		v2 = UnprojectZplane(v, -1, 0, &fDist2Plane);

		if (fDist2Plane >= 0.0f && fDist2Plane <= 1.0f)
		{
			m_objectModels[i].vecPos = v2;
		}
			

	}
	
}



void CD3d::CheckModelPositions()
{
	D3DXMATRIX matTransform;
	D3DXVECTOR4 out;
	D3DXVECTOR3 v, v2;
	CPoint screenCoord;
	float fDist2Plane;
	
	SetupMatricies();

	
	for (int i = 0; i < m_iNumObjects; i++)
	{
		matTransform = m_objectModels[i].matObjectModel*m_matView*m_matProjModel;

		/// check at each corner on the bounding box
		for (int k = 0; k < 8; k++)
		{

			bool repositionX = false;
			bool repositionY = false;

			v.x = (k&1) == 0 ? m_objectModels[i].fMinX : m_objectModels[i].fMaxX;
			v.y = ((k/2)&1) == 0 ? m_objectModels[i].fMinY : m_objectModels[i].fMaxY;
			v.z = ((k/4)&1) == 0 ? m_objectModels[i].fMinZ : m_objectModels[i].fMaxZ;

			D3DXVec3Transform(&out, &v, &matTransform);

			out.x /= out.w;
			out.y /= out.w;
			
			if (out.x > 1.0f) 
			{
				out.x = 1.0f;
				repositionX = true;
			}
			else if (out.x < -1.0f) 
			{
				out.x = -1.0f;
				repositionX = true;
			}

			if (out.y > 1.0f) 
			{
				out.y = 1.0f;
				repositionY = true;
			}
			else if (out.y < -1.0f)
			{
				out.y = -1.0f;
				repositionY = true;
			}

			if (repositionX || repositionY)
			{
				
				v2.x = out.x;
				v2.y = out.y;
			
				D3DXVec3Transform(&out, &v, &m_objectModels[i].matObjectModel);
			

				v2 = UnprojectZplane(v2, -1, out.z, &fDist2Plane);

				if (fDist2Plane >= 0.0f && fDist2Plane < 0.9f)
				{
					m_objectModels[i].vecPos.x += v2.x - out.x;
					m_objectModels[i].vecPos.y += v2.y - out.y;
				}

				SetupModelMatrix(i);

				matTransform = m_objectModels[i].matObjectModel*m_matView*m_matProjModel;
			}
		}

	}
	
}

void CD3d::EndGridDrag()
{

	for (int i = 0; i < m_iNumObjects; i++)
	{
		m_objectModels[i].bDragging = false;
	}

	m_bTiltingGrid = false;
}

void CD3d::ZoomGrid(CPoint screenCoord, long zDelta)
{

	HoldModelPositions_Begin();

	if (zDelta > 0)
		m_CalibInfo.fHeight /= 1.05f;
	else if (zDelta < 0)
		m_CalibInfo.fHeight *= 1.05f;
		
	m_CalibInfo.fHeight = min(200.0f, max(0.2f, m_CalibInfo.fHeight));
	

	HoldModelPositions_End();

//	CheckModelPositions();


	UpdateCalibInfo();
}

void CD3d::BeginRuler( CPoint screenCoord )
{

	float fDist2Plane;
	D3DXVECTOR3 v = UnprojectZplane(screenCoord, -1, 0, &fDist2Plane);

	if (fDist2Plane >= 0.0f && fDist2Plane <= 1.0f)
	{
		m_vRulerStartPos = v;
		m_vRulerEndPos = v;
		m_bRulerEnabled = true;
	}
}

void CD3d::EndRuler()
{
	m_bRulerEnabled = false;
}

void CD3d::RegisterCalibInfoCallback(_calib_callback_fxn _fxn, void* _usr_info)
{
	m_calib_callback_fxn = _fxn;
	m_callback_usr_info	= _usr_info;
}

void CD3d::UpdateCalibInfo()
{

// inform webpage
#ifndef _VMR_EXPORT
	((CAxVCACtrl*)(m_pParent))-> _OnChangeCalibparams(m_CalibInfo.fHeight, m_CalibInfo.fTilt, m_CalibInfo.fFOV, m_CalibInfo.fPan, m_CalibInfo.fRoll);
#endif

// inform callback
	if (m_calib_callback_fxn)
	{
		m_calib_callback_fxn(m_callback_usr_info, m_CalibInfo);
	}

}

D3DXVECTOR3 CD3d::UnprojectZplane(D3DXVECTOR3 logicalCoord, int modelIdx, float fZPlane, float *pfDist2Plane)
{

	D3DXMATRIX matTransform;  // complete transform
	D3DXMATRIX matInvTransform;
	float fDet;
	D3DXVECTOR3 v;
	D3DXVECTOR4 unprojnear, unprojfar, direction;
	unsigned int w, h;
	
	if (modelIdx == -1)
		matTransform = m_matView*m_matProjModel_Clipped;
	else
	{
		matTransform = m_objectModels[modelIdx].matObjectModel*m_matView*m_matProjModel_Clipped;
	}	

	m_GetClientWindowSize(&w, &h);

	/// convert to logical coords
	v.x = logicalCoord.x;
	v.y = logicalCoord.y;
	v.z = 0.0f;

	/// inverse transform matrix for unprojection
	D3DXMatrixInverse(&matInvTransform, &fDet, &matTransform);

	//// near clipping plane unprojection 
	D3DXVec3Transform(&unprojnear, &v, &matInvTransform);

	unprojnear.x /= unprojnear.w;
	unprojnear.y /= unprojnear.w;
	unprojnear.z /= unprojnear.w;

	/// far clipping plane projection
	v.z = 1.0f;
	D3DXVec3Transform(&unprojfar, &v, &matInvTransform);

	unprojfar.x /= unprojfar.w;
	unprojfar.y /= unprojfar.w;
	unprojfar.z /= unprojfar.w;

	/// find ray intersection point with plane z = fZPlane
	direction = unprojfar - unprojnear;

	float t = (fZPlane-unprojnear.z)/direction.z;
	float X = unprojnear.x + t*direction.x;
	float Y = unprojnear.y + t*direction.y;

	if (pfDist2Plane)
		*pfDist2Plane = t;


	return D3DXVECTOR3(X, Y, fZPlane);

	
}


D3DXVECTOR3 CD3d::UnprojectZplane(CPoint screenCoord, int modelIdx, float fZPlane, float *pfDist2Plane)
{
	D3DXVECTOR3 v;
	unsigned int w, h;	

	m_GetClientWindowSize(&w, &h);

	/// convert to logical coords
	v.x = 2.0f*(float)screenCoord.x/(float)w - 1.0f;
	v.y = -2.0f*(float)screenCoord.y/(float)h + 1.0f;
	v.z = 0.0f;

	return UnprojectZplane(v, modelIdx, fZPlane, pfDist2Plane);
	
}

D3DXVECTOR3 CD3d::UnprojectYplane(CPoint screenCoord, int modelIdx, float fYPlane, float *pfDist2Plane)
{

	D3DXMATRIX matTransform;  // complete transform
	D3DXMATRIX matInvTransform;
	float fDet;
	D3DXVECTOR3 v;
	D3DXVECTOR4 unprojnear, unprojfar, direction;
	unsigned int w, h;


	if (modelIdx == -1)
		matTransform = m_matView*m_matProjModel_Clipped;
	else
		matTransform = m_objectModels[modelIdx].matObjectModel*m_matView*m_matProjModel_Clipped;
		
		
	m_GetClientWindowSize(&w, &h);

	/// convert to logical coords
	v.x = 2.0f*(float)screenCoord.x/(float)w - 1.0f;
	v.y = -2.0f*(float)screenCoord.y/(float)h + 1.0f;
	v.z = 0.0f;

	/// inverse transform matrix for unprojection
	D3DXMatrixInverse(&matInvTransform, &fDet, &matTransform);

	//// near clipping plane unprojection 
	D3DXVec3Transform(&unprojnear, &v, &matInvTransform);

	unprojnear.x /= unprojnear.w;
	unprojnear.y /= unprojnear.w;
	unprojnear.z /= unprojnear.w;

	/// far clipping plane projection
	v.z = 1.0f;
	D3DXVec3Transform(&unprojfar, &v, &matInvTransform);

	unprojfar.x /= unprojfar.w;
	unprojfar.y /= unprojfar.w;
	unprojfar.z /= unprojfar.w;

	/// find ray intersection point with man plane (y = fYPlane)
	direction = unprojfar - unprojnear;

	float t = (fYPlane-unprojnear.y)/direction.y;
	float X = unprojnear.x + t*direction.x;
	float Z = unprojnear.z + t*direction.z;
	
	if (pfDist2Plane)
		*pfDist2Plane = t;


	return D3DXVECTOR3(X, fYPlane, Z);
	
}

D3DXVECTOR3 CD3d::UnprojectXplane(CPoint screenCoord, int modelIdx, float fXPlane, float *pfDist2Plane)
{

	D3DXMATRIX matTransform;  // complete transform
	D3DXMATRIX matInvTransform;
	float fDet;
	D3DXVECTOR3 v;
	D3DXVECTOR4 unprojnear, unprojfar, direction;
	unsigned int w, h;


	if (modelIdx == -1)
		matTransform = m_matView*m_matProjModel_Clipped;
	else
		matTransform = m_objectModels[modelIdx].matObjectModel*m_matView*m_matProjModel_Clipped;
		
		
	m_GetClientWindowSize(&w, &h);

	/// convert to logical coords
	v.x = 2.0f*(float)screenCoord.x/(float)w - 1.0f;
	v.y = -2.0f*(float)screenCoord.y/(float)h + 1.0f;
	v.z = 0.0f;

	/// inverse transform matrix for unprojection
	D3DXMatrixInverse(&matInvTransform, &fDet, &matTransform);

	//// near clipping plane unprojection 
	D3DXVec3Transform(&unprojnear, &v, &matInvTransform);

	/// unhomo divide
	unprojnear.x /= unprojnear.w;
	unprojnear.y /= unprojnear.w;
	unprojnear.z /= unprojnear.w;

	/// far clipping plane projection
	v.z = 1.0f;
	D3DXVec3Transform(&unprojfar, &v, &matInvTransform);

	/// unhomo divide
	unprojfar.x /= unprojfar.w;
	unprojfar.y /= unprojfar.w;
	unprojfar.z /= unprojfar.w;

	/// find ray intersection point with man plane (x = fXPlane)
	direction = unprojfar - unprojnear;

	float t = (fXPlane-unprojnear.x)/direction.x;
	float Y = unprojnear.y + t*direction.y;
	float Z = unprojnear.z + t*direction.z;
	
	if (pfDist2Plane)
		*pfDist2Plane = t;

	return D3DXVECTOR3(fXPlane, Y, Z);
	
}

BOOL CD3d::m_SetVideoCaptureSize( int iWidth, int iHeight )
{
	if ( ( iWidth > 0 ) && ( iHeight > 0 ) )
	{
		m_csVideoSize.cx = iWidth;
		m_csVideoSize.cy = iHeight;
		return TRUE;
	}
	else
		return FALSE;
}
#endif // _VT
