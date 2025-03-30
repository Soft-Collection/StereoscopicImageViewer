#ifndef __CAUDIOVIDEORENDER_H__
#define __CAUDIOVIDEORENDER_H__

#include "CImage.h"
#include "CComPort.h"
#include "CAutoMemoryBase.h"
#include "CAutoMemoryDib.h"
#include "CAutoMemoryDirect2D.h"
#include "CAutoMemoryDirect3D.h"
#include "../VideoCommon/CCriticalSectionPool.h"
#include <list>

class CAudioVideoRender
{
public:
	enum eAudioVideoRenderErrors : int
	{
		NoError = 0,
		NullHandle = 1,
		CouldNotCreateInstance = 2,
		CouldNotOpenVideoDecompressor = 3,
		CouldNotCloseVideoCodec = 4,
		CouldNotSetVideoDecompressParam = 5,
		CouldNotVideoDecompress = 6,
		ExceptionInVideoReInit = 7,
		ExceptionInVideoRender = 8,
		DibIsNull = 9,
		Direct2DIsNull = 10,
		Direct3DIsNull = 11,
		DecodedVideoIsNull = 12,
		DecodedVideoNotReady = 13,
		ExceptionInAudioReInit = 14,
		CouldNotOpenWaveOut = 15,
		CouldNotCloseWaveOut = 16,
		CouldNotWriteToWaveOut = 17,
		WaveIsNull = 18,
		UUCoreIsNull = 19,
		DifferentLeftRightImageDimensions = 20
	};
	enum eVideoRenderTargets : int
	{
		GDI = 0,
		D2D = 1,
		D3D = 2
	};
	enum eFrequencies : int
	{
		Default = 0,
		F1Hz = 1,
		F60Hz = 2,
		F75Hz = 3,
		F100Hz = 4,
		F120Hz = 5,
		F144Hz = 6
	};
	enum eSignalSources : int
	{
		ScreenSensor = 0,
		COMPort = 1
	};
private:
	enum eCriticalSections : int
	{
		DecodedFrameCS = 0,
		Size = 1 //Must be last.
	};
private:
	CCriticalSectionPool* mCriticalSectionPool;
	//----------------------------------------
	HWND mHWnd;
	eVideoRenderTargets mRenderTarget;
	eFrequencies mFrequency;
	eSignalSources mSignalSource;
	std::wstring mComPortName;
	CComPort* mComPort;
	CAutoMemoryBase* mAutoMemoryBase;
	//----------------------------------------
	CImage* mLeftImage;
	CImage* mRightImage;
	//----------------------------------------
	bool mImageToPlayIsLeft;
public:
	CAudioVideoRender(HWND hWnd, eVideoRenderTargets renderTarget, eFrequencies frequency, eSignalSources signalSource, LPCWSTR comPort, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath);
	~CAudioVideoRender();
private:
	eAudioVideoRenderErrors DrawWindow();
	int GetRefreshRate();
public:
	eAudioVideoRenderErrors VideoRender();
};
#endif // __CAUDIOVIDEORENDER_H__