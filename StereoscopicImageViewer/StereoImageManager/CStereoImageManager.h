#ifndef __CSTEREOIMAGEMANAGER_H__
#define __CSTEREOIMAGEMANAGER_H__

#include "CImage.h"
#include "CComPort.h"
#include "../StereoRendering/CStereoDirect3D.h"
#include "../Common/CCriticalSectionPool.h"
#include <list>
#include <chrono>

class CStereoImageManager
{
public:
	enum eStereoImageManagerErrors : int
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
	enum eFrequencies : int
	{
		Default = 0,
		Test = 1
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
	eFrequencies mFrequency;
	eSignalSources mSignalSource;
	std::wstring mComPortName;
	CComPort* mComPort;
	CStereoDirect3D* mStereoDirect3D;
	//----------------------------------------
	CImage* mLeftImage;
	CImage* mRightImage;
	//----------------------------------------
	bool mImageToPlayIsLeft;
	bool mFirstFrameAlreadyArrived;
	UINT32 mFrameCounter;
	std::chrono::time_point<std::chrono::high_resolution_clock> mMeasureTimeFromFirstFrame;
public:
	CStereoImageManager(HWND hWnd, eFrequencies frequency, eSignalSources signalSource, LPCWSTR comPort, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath);
	~CStereoImageManager();
private:
	int GetRefreshRate();
public:
	eStereoImageManagerErrors VideoRender();
	eStereoImageManagerErrors SetGlassesTimeOffset(int offset);
	eStereoImageManagerErrors SetTransparentTimePercent(int percent);
};
#endif // __CSTEREOIMAGEMANAGER_H__