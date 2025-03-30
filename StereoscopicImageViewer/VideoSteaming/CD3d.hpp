
#ifndef _CD3D_HPP_
#define _CD3D_HPP_

#define D3D_OVERLOADS
#include <math.h>
#include <d3d.h>
#include <d3dx.h>
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef _VT
#include "triangulation.h"		// Triangulation of polygons, for drawing of any shape of polygons using D3D
#include "D3DFile/D3DFile.h"
#include "vca_calibdef.h" 
//_______________________________________
//	Value used to increase the radius when drawing circular arcs
#define RADIUS_STEP					0.1
#endif

// D3D Canvas Size
//#define	_MAXWIDTH_D3DCANVAS			(720)		// 0	current display mode resolution
//#define	_MAXHEIGHT_D3DCANVAS		(576)		// 0	current display mode resolution
#define	_MAXWIDTH_D3DCANVAS			(1024)		// 0	current display mode resolution
#define	_MAXHEIGHT_D3DCANVAS		(1024)		// 0	current display mode resolution

// Image TextureSurface Size
//#define	_MAXWIDTH_D3DTEXTURE		(720)
//#define	_MAXHEIGHT_D3DTEXTURE		(576)
#define	_MAXWIDTH_D3DTEXTURE		(1024)
#define	_MAXHEIGHT_D3DTEXTURE		(1024)

#ifdef _DRAWRECORDS_ALL
#define MAX_NUM_RECORDS			(1024)
#define MAX_TEXT_BUFFER			(1024*20)
#define MAX_POLYGON_BUFFER		(512)
#define MAX_LINEPOINTS_BUFFER	(1024*10)
#endif

#ifdef _VT
#define MAXCOUNT_POLYGONEDGE		40		// Definitely we need more than 6 polygon points for each polygon :D
#define MAXCOUNT_BRUSHTEXTURES		2
#define BRUSH_TEXTURESIZE			16.0f
#else
#define MAXCOUNT_POLYGONEDGE		6
#endif

#define _D3D_DEFAULT_FONT_SIZE		52

#ifdef _VT
#define MAXNUM_OBJECTS 6
#define NUM_COLOURS 6
typedef struct
{
	CD3DFile* pFileObject;

	D3DXMATRIX matObjectModel;

	D3DXVECTOR3 vecRot;
	D3DXVECTOR3 vecPos;
	D3DXVECTOR3 vecPickPos;
	D3DXVECTOR2 vecLogicalScreenPos;

	bool		bDragging;
	bool		bHighlighted;
	float		fScale;

	float		fMaxZ, fMinZ;
	float		fMaxX, fMinX;
	float		fMaxY, fMinY;

}ObjectModel;

typedef void(*_calib_callback_fxn)(void* /* _usr_info */, VCA_CALIB_INFO_T_D );

#endif // _VT

class CD3d
{
public:
	typedef struct _tagDDADAPTERINFO
	{
		BOOL	  fIsAvailableGUID;
		GUID	  tGUID;
		TCHAR	  szDesc[255];
		TCHAR	  szName[255];
		HMONITOR  hMonitor;
	}tDDADAPTERINFO;

	typedef enum _tagCOLORFORMAT
	{
		CF_NOTDEFINED = 0,
		CF_YUY2,
		CF_BGRX,	//rgb32
		CF_BGR,		//rgb24
		CF_BGR565,
		CF_BGR555,
		CF_YV12,
		CF_IYUV
	}eCOLORFORMAT;

	static DDPIXELFORMAT DDPF_RGBA16;
	static DDPIXELFORMAT DDPF_RGB32;
	static DDPIXELFORMAT DDPF_RGB24;
	static DDPIXELFORMAT DDPF_RGB565;
	static DDPIXELFORMAT DDPF_RGB555;
	static DDPIXELFORMAT DDPF_YUY2;
#ifdef _VT
	static DDPIXELFORMAT DDPF_RGBA32;
#endif // _VT

	typedef struct _tagMYPOLYGON
	{
		ULONG	cnt_point;
		POINT	atpoint[MAXCOUNT_POLYGONEDGE];
	}MYPOLYGON;

public:	
	CD3d(void* parent);
	~CD3d();

#ifdef _VMR_EXPORT 
	// Note: for VCAMetaRender.dll, we do not need primary DDS, we only get the backbuffersurface from API
	BOOL	m_SetBackSurface( LPDIRECTDRAWSURFACE7 pddsBack );
	VOID	m_SetClientWindowSize( RECT rcCanvas )	{ m_rcCanvas = rcCanvas; } ;
	RECT	m_rcCanvas;
#endif 
	INT	 m_GetManyDDAdapters();
	tDDADAPTERINFO* m_GetDDAdapterInfo( INT _nDDAdapterNo );	// _nDDAdapterNo; Zerobase

	HRESULT m_Setup( INT			_nUseDDAdapterNo,
					 eCOLORFORMAT	_eD3dTextureCF = CF_BGR565, 
					 HWND			_hwndWindow = NULL,
					 INT			_n3DCanvasWidth   = _MAXWIDTH_D3DCANVAS,
					 INT		    _n3DCanvasHeight  = _MAXHEIGHT_D3DCANVAS,
					 INT			_n3DTextureWidth  = _MAXWIDTH_D3DTEXTURE,
					 INT		    _n3DTextureHeight = _MAXHEIGHT_D3DTEXTURE
#ifdef _VMR_EXPORT
					 ,LPDIRECTDRAWSURFACE7 pddsBack = NULL
#endif					 
					 );
	BOOL m_Endup();

	BOOL m_SetDDClipperWindow( HWND _hwnd );
	HWND m_GetDDClipperWindow();

	SIZE m_GetCurrent3DCanvasSize();
	SIZE m_GetCurrent3DTextureSize();

	BOOL m_GetTextExtent( LPCSTR _lpszText, SIZE* _psizePixel
#ifdef _VT
					,float		_fScale = 1.0f
#endif // _VT		
		);

	/*
	3DCanvas BackBuffer에 영상을 뿌려 넣는다. 이는 TextureSurface를 이용하여 Attach된 BackBuffer에
	3DDevice가 자동으로 매핑되어져 뿌려지게 된다. 이 단계에서 _rc3DCanvas 대비 _sizeSrcImage 값에 따라
	ScaleUp/Down도 발생하게 된다.

	_rc3DCanvas   ; 3DCanvas상의 좌표계(x1,y1,x2,y2)를 입력한다.
	_sizeSrcImage ; 뿌릴 영상의 크기
	*/
	BOOL m_DrawImage( RECT			_rc3DCanvas,		// Target
					  SIZE			_sizeSrcImage,		// Src
					  eCOLORFORMAT	_eCFSrcImage,		// CF_YUY2, CF_YV12,
					  BYTE* const	_pImage );	

	/*
	Texture Buffer에 하나 또는 여러개의 BITMap를 그린다.
	뿌려지는 단계를 고려 했을때, 위에 m_DrawImage를 한 다음에 뿌려야지 된다. 안그러면 영상에 의해 지워진다.
	*/
	#define MAXBMPICON 10
	typedef struct _tagBMPIcon
	{
		RECT			_rc3DCanvas;
		SIZE			_sizeSrcImage;
		eCOLORFORMAT	_eCFSrcImage;
		BYTE*			_pImage;
	}tMYICON;

	tMYICON tMyIcon[MAXBMPICON];

#ifdef _VT
	typedef enum _tagBRUSHTYPE
	{
		BRT_SOLID		= 0,
		BRT_HATCH,
	}eBRUSHTYPE;
#endif // _VT

	BOOL m_DrawBitmap(int iCnt );

	/*
	Texture Buffer에 하나 또는 여러개의 속이 채워진 RECT를 뿌려 넣는다.
	뿌려지는 단계를 고려 했을때, 위에 m_DrawImage를 한 다음에 뿌려야지 된다. 안그러면 영상에 의해 지워진다.

	_rcTarget   ; ClipperWindow와는 상관없이 뿌리고자하는 ScreenCoordinate좌표계(x1,y1,x2,y2)
	_sizeSrcImage  ; 뿌릴 영상의 크기
	_apRcList   ; RECT Array
	_nRectCount ; Array의 길이 (하나 또는 여러개의 RECT) 
	_cbAlpha	; 0(Transparent) ~ 255(Opaque)
	_clrRect    ; RECT Color
	*/
	BOOL m_DrawRectList( RECT		 _rc3DCanvas,
						 SIZE		 _sizeSrcImage,
						 RECT* const _apRcList,
						 INT		 _nRectCount,
						 BYTE		 _cbAlpha,
						 COLORREF	 _clrRect );

	BOOL m_DrawPolygonList( RECT				_rc3DCanvas,
					        SIZE				_sizeSrcImage,
						    MYPOLYGON* const	_aptPolygonList,
						    INT					_nPolygonCount,
#ifdef _VT
							eBRUSHTYPE			_brushType,
#endif // _VT
						    BYTE				_cbAlpha,
						    COLORREF			_clrRect );
	/*
	Texture Buffer에 하나 또는 여러개의 속이 채워진 RECT를 뿌려 넣는다.
	뿌려지는 단계를 고려 했을때, 위에 m_DrawImage를 한 다음에 뿌려야지 된다. 안그러면 영상에 의해 지워진다.

	_rcTarget   ; ClipperWindow와는 상관없이 뿌리고자하는 ScreenCoordinate좌표계(x1,y1,x2,y2)
	_sizeSrcImage  ; 뿌릴 영상의 크기
	_apPtList   ; POINT Array
	_nPtCount   ; Array의 길이 (하나 또는 여러개의 POINT) 
	_cbAlpha	; 0(Transparent) ~ 255(Opaque)
	_clrLine    ; Line Color
	*/
	BOOL m_DrawLineList( RECT			_rc3DCanvas,
						 SIZE			_sizeSrcImage,
						 POINT* const	_apPtList,
						 INT			_nPtCount,
						 BYTE			_cbAlpha,
						 COLORREF		_clrLine );

	BOOL m_DrawText( RECT		_rc3DCanvas,
				     SIZE		_sizeSrcImage,
				     POINT      _ptStartXY,
				     LPCSTR     _lpszText,
				     BYTE		_cbAlpha,
				     COLORREF	_clrFont
#ifdef _VT
					,float		_fScale = 1.0f
#endif // _VT
										);

#ifdef _DRAWRECORDS_ALL
	BOOL m_DrawAllRecords();
#endif

	BOOL m_CopyRectIn3DCanvas( RECT& rcNew, RECT& rcOld );
	BOOL m_Clear( RECT tr );

	INT  m_GetAdapterNoByPosition( RECT const _rcScreen );
	RECT m_AdjustPositionByAdapterNo( RECT const _rcScreen, INT _nNo );

	/*
	3DCanvas, 즉 Backbuffer의 내용을 Primary로 표현한다.
	이단계에서도 ScaleUp/Down이 발생한다.
	
	_prcScreen ; NULL 이면 Clipping Window의 크기만큼 뿌린다.
	*/
	BOOL m_PrimaryPresentation( RECT* const _prcScreen, RECT* const _prc3DCanvas );

private:
	typedef enum _tagSURFACETYPE
	{
		SURFACETYPE_INVALID			= 0,
		SURFACETYPE_AGPTEXTURE		= 1,
		SURFACETYPE_VIDMEMTEXTURE	= 2,
		SURFACETYPE_3DCANVAS		= 3,
		SURFACETYPE_PRIMARY			= 4,
		SURFACETYPE_MANAGEDTEXTURE	= 5,
	}eSURFACETYPE;

	typedef enum _tagDRAWRECORDTYPE
	{
		DRT_POINTLIST	= D3DPT_POINTLIST,	// 1
		DRT_LINELIST	= D3DPT_LINELIST,	// 2
		DRT_LINESTRIP	= D3DPT_LINESTRIP,	// 3

		DRT_IMAGE		= 7,
		DRT_TEXT		= 8,
		DRT_RECTLIST	= 9,
		DRT_POLYGONLIST	= 10,
		DRT_BITMAP		= 11,
#ifdef _VT
		DRT_CIRCLE		= 12,
		DRT_CIRCULARARC	= 13,
#endif // _VT
	}eDRAWRECORDTYPE;

	typedef struct _tagDRAWRECORD
	{
		eDRAWRECORDTYPE drawType;
		DWORD			colorValue;
		SIZE			srcSize; //source size or source dimension...
#ifdef _VT
		eBRUSHTYPE		brushType;
#endif // _VT
		union {
			//image
			struct {
				eCOLORFORMAT colorFormat;
				BYTE* pSrc;
				RECT  roi;
				DWORD flag;
			}img;

			//POLYGONs
			struct {
				ULONG		nPolygon;
				MYPOLYGON*	aptPolygon;
			}polygons;

			//RECTs
			struct {
				ULONG nRect;
				RECT* rects;
			}rects;

			//LINE /POINT 물리적인 point의 수를 나타낸다...
			struct {
				ULONG  nPt;
				POINT* pts;
			}pts;

			//text
			struct {
				LPCSTR pText;
				POINT  pt;		// start XY
#ifdef _VT
				float	fScale;
#endif // _VT
			}text;

#ifdef _VT
			//circle
			struct {
				POINT	ptCentre;		// centre of circle
				double	fRadius;		// radius
				BOOL	bFill;
			}circle;

			struct {
				POINT			ptCentre;		// centre of circle
				double			fRadius;		// radius
				unsigned short	usWidth;		// The width of the arc
				double			dStartRadius;
				double			dFinishRadius;
			}circularArc;
#endif // _VT
		};
	}tDRAWRECORD;

private:
	// Error Message
	#define _MACRO_DDERROR( hrDDErr, lpszAPI ) _DisplayDDError( hrDDErr, lpszAPI, __FILE__, __LINE__ )
	HRESULT	_DisplayDDError( HRESULT _hrDDErr, LPTSTR _lpszAPI, LPTSTR _lpszFile, INT _nLineNo );

	HRESULT _EnumDDAdapter();
	static BOOL WINAPI _lpWrapAdapterEnumCallback( GUID*	_pGUID,
												   LPSTR	_strDesc,
												   LPSTR	_strName,
												   VOID*	_lpContext,
												   HMONITOR	_hMonitor );
	BOOL _AdapterEnumCallback( GUID*	_pGUID,
							   LPSTR	_strDesc,
							   LPSTR	_strName,
							   HMONITOR	_hMonitor );

	static HRESULT WINAPI _lpEnumSurfacesCallback2( LPDIRECTDRAWSURFACE7	lpDDSurface,
												    LPDDSURFACEDESC2		lpDDSurfaceDesc,
													LPVOID					lpContext );

#ifdef _VT
	static HRESULT WINAPI _lpEnumSurfacesCallback3( LPDIRECTDRAWSURFACE7	lpDDSurface,
												   LPDDSURFACEDESC2		lpDDSurfaceDesc,
												   LPVOID				lpContext );
#endif // _VT

	static HRESULT WINAPI _lpEnumZBufferCallback( DDPIXELFORMAT* pddpf, VOID* pddpfDesired );

#ifdef _VMR_EXPORT
	BOOL	_InitDDInterface( LPDIRECTDRAWSURFACE7 lpDDSurface, HWND _hwndWindow );
#endif
	BOOL	_InitDDInterface( INT _nUseDDAdapterNo, HWND _hwndWindow );
	VOID	_DeinitDDInterface();

	BOOL	_InitPrimarySurface();
	BOOL	_Init3DDeviceAnd3DCanvasSurface( INT _nWidth, INT _nHeight );
	BOOL	_InitImageTextureSurface( INT _nWidth, INT _nHeight, eCOLORFORMAT _eD3dTextureCF );
	VOID	_DeinitSurface();

	// _dwFlags;
	#define D3DFONT_BOLD			0x0001
	#define D3DFONT_ITALIC			0x0002
	#define D3DFONT_SHADOW_LEVEL1	0x0004
	#define D3DFONT_SHADOW_LEVEL2	0x0008
	#define D3DFONT_SHADOW_LEVEL3	0x0010
	#define D3DFONT_ANTIALIAS		0x0020
	BOOL _InitFont( LPCSTR _lpszFontName, DWORD _dwHeight, DWORD _dwFlags );
	BOOL _DeinitFont();

	LPDIRECTDRAWSURFACE7 _CreateCustomSurface( LPDIRECTDRAW7 _pDD,
											   eSURFACETYPE  _surfaceType,
											   PVOID		 _lpParam,
											   DWORD		 _dwWidth,
											   DWORD		 _dwHeight );

	// m_apddsTextures[8] 중에서 어떤것을 사용할것인지를 결정한다.
	// 결과는 m_dwTxWidth, m_dwTxHeight, m_pddsTexture를 사용한다.
	BOOL _FindSuitableTextureSurface( INT _nTargetWidth, INT _nTargetHeight );

	BOOL _GetSurfacePointer( LPDIRECTDRAWSURFACE7 const _pDDSurface,
						     RECT* const				_prcTarget,
							 BYTE**						_ppSurf,
							 DWORD*						_pdwPitch );
	BOOL _ReleaseSurfacePointer( LPDIRECTDRAWSURFACE7 const _pDDSurface );

	void _EnableAlpha();

	//D3DVECTOR
	void _SetVertexScreenXYZ( D3DTLVERTEX* _aptVertex, int indexFrom, int count, D3DVECTOR _tVector );
	void _SetVertexScreenXY( D3DTLVERTEX* _aptVertex, int indexFrom, int count, D3DVALUE _foX, D3DVALUE _foY );
	void _SetVertexZValue( D3DTLVERTEX* _aptVertex, int indexFrom, int count, D3DVALUE _foZValue );
	void _SetVertexColorValue( D3DTLVERTEX* _aptVertex, int indexFrom, int count, DWORD colorValue );
	void _SetVertexTextureXY( D3DTLVERTEX* _aptVertex, int indexFrom, int count, D3DVALUE _foU, D3DVALUE _foV );

#ifdef _DRAWRECORDS_ALL
	BOOL _PushRecord( RECT rcCanvas, tDRAWRECORD *pRecord );
	BOOL _ClearAllRecords();
#endif
	
	BOOL _DrawRecords( RECT			_rcTarget,
					   D3DVALUE		_foZValue,
					   ULONG		_nRecordCount,
					   tDRAWRECORD*	_aptDrawRecords );
	BOOL _DrawImageTexture( RECT _rcTarget, RECT _rcSrc, DWORD _dwColor, D3DVALUE _foZValue );

	//	ColorForamt Converter
	void _InitColorConversionFunctions();
	BOOL _ConvertColorFormat( BYTE*			_pSrc,
							  eCOLORFORMAT  _eCFSource,
							  SIZE			_sizeSource,
							  BYTE*			_pDst,
							  eCOLORFORMAT  _eCFDest,
							  DWORD			_dwDstPitch,
							  DWORD			_dwScale_factor );
public:
	LPDIRECTDRAWSURFACE7 m_GetBackDDS() { return m_pddsBackbuffer; };

private:
	LPDIRECTDRAW7		 m_pDD;

	LPDIRECT3DDEVICE7    m_pd3dDevice;
	DWORD				 m_dw3DCanvasWidth;			// D3D Canvas의 크기
	DWORD				 m_dw3DCanvasHeight;

	LPDIRECTDRAWSURFACE7 m_pddsPrimary;
	LPDIRECT3D7          m_pD3D;
	LPDIRECTDRAWSURFACE7 m_pddsBackbuffer;
	LPDIRECTDRAWCLIPPER  m_pddClipper;
	HWND				 m_hwndClipper;

	eCOLORFORMAT		 m_eD3dTextureCF;
	LPDIRECTDRAWSURFACE7 m_apddsTextures[8];	// 임시로 여러개를 미리 만들어 둘때 사용

	LPDIRECTDRAWSURFACE7 m_pddsTexture;
	DWORD				 m_dwTextureWidth;
	DWORD				 m_dwTextureHeight;

	#define				MAX_TLVERTEXT_IMAGE			(4)
	D3DTLVERTEX*		m_aptImageTLV;		// Triangle Vertext for Image

#ifdef _DRAWRECORDS_ALL
	// we need more vertex when drawing blobs in one go, currently max blob is 200*160 rects, so max vertex is 200*160*6=0x2EE00
	#define				MAX_TLVERTEXT_RECTANDLINES	(0x30000)
#else
	#define				MAX_TLVERTEXT_RECTANDLINES	(0x4000)
#endif
	D3DTLVERTEX*		m_aptRectAndLineTLV;// Triangle Vertext for RECT and LINES

	#define				MAX_TLVERTEXT_FONT			(50 * 6)
	D3DTLVERTEX*		m_aptFontTLV;		// Triangle Vertext for Font

	//
	INT					m_nUseDDAdapterNo;
	tDDADAPTERINFO*		m_aptDDAdapterInfo;
	INT					m_nDDAdapterCount;

	CRITICAL_SECTION	m_crit;

#ifdef _DRAWRECORDS_ALL
	tDRAWRECORD			*m_pRecords;
	TCHAR				*m_pTexts[MAX_NUM_RECORDS+1];
	MYPOLYGON			*m_pPolygons[MAX_NUM_RECORDS+1];
	POINT				*m_pLinePoints[MAX_NUM_RECORDS+1];
	INT					m_nNumTexts;
	INT					m_nNumPolygons;
	INT					m_nNumRecords;
	INT					m_nNumLines;
	RECT				m_rcDrawingCanvas;
#endif

private:// About Font
	DWORD	_GetFontAreaHeight( HFONT _hFont, DWORD _dwWidth, int _nMargin_x, int _nMargin_y );
	BOOL	_MakeStateBlock( LPDIRECTDRAWSURFACE7 const _pDDSurface );

	D3DTLVERTEX _InitFontVertex( const D3DXVECTOR4& p, D3DCOLOR color, float tu, float tv );
	BOOL	_DrawText( float x, float y,RECT rc, DWORD dwColor,const char* strText, 
#ifdef _VT
		float fScale = 1.0f,
#endif // _VT
		int Space=0 );

private:// About Font
	INT m_iUserFontSize;
	int		m_def_hint;
	DWORD	m_dwFontWidth;
	DWORD	m_dwFontHeight;
	float	m_fTexCoords[127-32+1][4];

	LPDIRECTDRAWSURFACE7	m_pddsFontTexture;

	DWORD	m_dwSavedStateBlock;
	DWORD	m_dwDrawTextStateBlock;
	BOOL	m_IsAllocStateBlock;

	void*	m_pParent;

#ifdef _VT
public:
	BOOL m_DrawCircle(	RECT		_rc3DCanvas,
						SIZE		_sizeSrcImage,
						POINT		 _ptCentre,
						double		_fRadius,
						BOOL		_bFill,
						BYTE		_cbAlpha,
						COLORREF	_clrCircle );

	BOOL m_DrawCircularArc(	RECT		_rc3DCanvas,
							SIZE		_sizeSrcImage,
							POINT		 _ptCentre,
							double		_fRadius,
							unsigned short _usWidth, 
							double		_startRadius,
							double		_finishRadius,
							BYTE		_cbAlpha,
							COLORREF	_clrCircle );

	HRESULT m_GetClientWindowSize( unsigned int *p_uiClientWidth, unsigned int *p_uiClientHeight );

	void	m_SetFontSize( int iFontSize );
	INT		GetFontSize() { return m_iUserFontSize; };
	BOOL	m_SetVideoCaptureSize( int iWidth, int iHeight );

private:
	BOOL	_DrawPolygonVCA( RECT			_rc3DCanvas,
							 D3DVALUE		_foZValue,
							 ULONG			_nRecordCount,
							 tDRAWRECORD*	_aptDrawRecords );
	ULONG	_Triangulate( RECT _rc3DCanvas, tDRAWRECORD*	_aptDrawRecords );

	BOOL	_UpdateBrushAlpha( eBRUSHTYPE brushType, BYTE _cbAlpha );
	BOOL	_InitBrushTexture( eBRUSHTYPE brushType, UINT bitmapId );
	BOOL	_DeinitBrushTexture( eBRUSHTYPE brushType );

private:
	BOOL	m_bFontChanged;

	LPDIRECTDRAWSURFACE7	m_pddsBrushTextures[MAXCOUNT_BRUSHTEXTURES];

/////// All 3D calibation drawing stuff ////////////////////
public:
	void RenderCalibrationGrid();
	void DragGrid(CPoint screenCoord);
	void PickGrid(CPoint screenCoord);
	void ZoomGrid(CPoint screenCoord, long zDelta);
	void EndGridDrag();
	void MouseOver(CPoint screenCoord);
	void SetInitialCameraParams(VCA_CALIB_INFO_T_D& calibInfo);
	void SetCameraParams(VCA_CALIB_INFO_T_D& calibInfo, bool bCentre);
	void GetCameraParams(VCA_CALIB_INFO_T_D& calibInfo );
	void PauseVideo(bool bPaused);
	void BeginRuler( CPoint screenCoord );
	void EndRuler();
	float GetRulerLength();
	void RegisterCalibInfoCallback(_calib_callback_fxn, void* _usr_info);
	void EnableCalibDrawSky(BOOL enable);
	void InsertObjectModel( CPoint screenCoord );
	void TransparentGrid(BOOL enable);

private:

	HRESULT InitGridMesh();
	HRESULT DeInitGridMesh();
	BOOL CD3d::InitTextures();

	void DrawGroundPlane();
	void DrawPerson(int modelIdx);
	void DrawObject(int modelIdx);
	void DrawRuler();

	BOOL LoadXFile(LPCSTR strFileName, ObjectModel *pModel);
	static BOOL CalcFileObjectSizeCB( CD3DFileObject* pObject, D3DMATRIX* pmat, VOID* pContext );

	D3DXVECTOR3 UnprojectXplane(CPoint screenCoord, int modelIdx = -1, float fXPlane = 0.0f, float *pfDist2Plane = NULL);
	D3DXVECTOR3 UnprojectYplane(CPoint screenCoord, int modelIdx = -1, float fYPlane = 0.0f, float *pfDist2Plane = NULL);
	D3DXVECTOR3 UnprojectZplane(CPoint screenCoord, int modelIdx = -1, float fZPlane = 0.0f, float *pfDist2Plane = NULL);
	D3DXVECTOR3 UnprojectZplane(D3DXVECTOR3 logicalCoord, int modelIdx = -1, float fZPlane = 0.0f, float *pfDist2Plane = NULL);

	void SetupMatricies();
	void SetupModelMatrix(int iModelIdx);
	void CheckModelPositions();
	ObjectModel* PickObject(CPoint screenCoord, D3DXVECTOR3 *pPickedPos);
	void ClipCoords(CPoint &screenCoord);
	void UpdateCalibInfo();	
	BOOL LoadTextureFromResource(LPDIRECTDRAWSURFACE7 *ppddsTexture, DWORD idRes, DWORD bmpsize);
	BOOL _insertObjectModel(D3DXVECTOR3 pos);
	void DrawShadow(ID3DXSimpleShape *pMesh, D3DMATRIX matWorld);
	void HoldModelPositions_Begin();
	void HoldModelPositions_End();

	D3DMATERIAL7 m_materialShadow;

	ID3DXSimpleShape *m_pSphereMesh, *m_pCylinderMesh, *m_pConeMesh;

	ObjectModel m_objectModels[MAXNUM_OBJECTS];
	int			m_iNumObjects;

	D3DXMATRIX m_matView, m_matView_noTrans, m_matProjImage, m_matProjModel, m_matProjModel_Clipped, m_matShadow;

	D3DXVECTOR3 m_vecGridRot;

	D3DVIEWPORT7 m_mainViewport;
	D3DMATERIAL7 m_materialPerson[MAXNUM_OBJECTS];
	D3DLIGHT7    m_light;
	LPDIRECTDRAWSURFACE7 m_pddsGroundTexture;
	LPDIRECTDRAWSURFACE7 m_pddsSkyboxTextures[6];
	LPDIRECTDRAWSURFACE7 m_pddsRulerTexture;

	D3DVERTEX m_vGroundVerticies[4];
	D3DVERTEX m_vSkyboxVerticies[6][4];
	D3DLVERTEX  m_vHorizonVerticies[5];

	static D3DCOLORVALUE  m_colourLUT[NUM_COLOURS];

	SIZE  m_csVideoSize; /// native video dimensions
//	float m_fCameraHeight;
	float m_fHumanHeight;
//	float m_fYFOV_deg;
//	float m_fTiltAngle_deg;
//	float m_fScale_fact;
//	float m_fPanAngle_deg;

	VCA_CALIB_INFO_T_D m_CalibInfo;

	BOOL  m_bTiltingGrid;
	CPoint m_tiltStartPos;
	
	BOOL m_bVideoPaused;
	BOOL m_bParamsSet;
	BOOL m_bSkyEnabled;
	BOOL m_bTransparentGrid;
	BOOL m_bRulerEnabled;
	D3DXVECTOR3 m_vRulerStartPos, m_vRulerEndPos;

	D3DDEVICEDESC7 m_d2dDevDesc;

	_calib_callback_fxn m_calib_callback_fxn;
	void * m_callback_usr_info;

#endif // _VT
};

#endif	//#ifndef _CD3D_HPP_
