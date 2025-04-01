#include "stdafx.h"
#include "CAudioVideoRender.h"
#include "../VideoCommon/CExceptionReport.h"
#include "../VideoCommon/CCriticalSectionPool.h"
#include <chrono>
#include <thread>
#include <Windows.h>
#include <iostream>

CAudioVideoRender::CAudioVideoRender(HWND hWnd, eVideoRenderTargets renderTarget, eFrequencies frequency, eSignalSources signalSource, LPCWSTR comPort, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath)
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::Size);
	mCriticalSectionPool->Enter(eCriticalSections::DecodedFrameCS);
	//----------------------------------------------------
	mHWnd = hWnd;
	mAutoMemoryBase = NULL;
	mRenderTarget = renderTarget;
	if (mRenderTarget == eVideoRenderTargets::GDI) mAutoMemoryBase = new CAutoMemoryDib();
	else if (mRenderTarget == eVideoRenderTargets::D2D) mAutoMemoryBase = new CAutoMemoryDirect2D();
	else if (mRenderTarget == eVideoRenderTargets::D3D) mAutoMemoryBase = new CAutoMemoryDirect3D();
	mFrequency = frequency;
	mSignalSource = signalSource;
	mComPortName = std::wstring(comPort);
	mComPort = NULL;
	if (mSignalSource == eSignalSources::COMPort)
	{
		mComPort = new CComPort();
	}
	//----------------------------------------------------
	mLeftImage = new CImage();
	mLeftImage->FilePath = std::wstring(leftImageFilePath);
	mLeftImage->IsLeft = true;
	mLeftImage->RectangleHeight = 30;
	mLeftImage->RectanglesMustBeDrawn = (mSignalSource == eSignalSources::ScreenSensor);
	CImage::LoadPNG(mLeftImage);
	mRightImage = new CImage();
	mRightImage->FilePath = std::wstring(rightImageFilePath);
	mRightImage->IsLeft = false;
	mRightImage->RectangleHeight = 30;
	mRightImage->RectanglesMustBeDrawn = (mSignalSource == eSignalSources::ScreenSensor);
	CImage::LoadPNG(mRightImage);
	//----------------------------------------------------
	mImageToPlayIsLeft = true;
	//----------------------------------------------------
	mCriticalSectionPool->Leave(eCriticalSections::DecodedFrameCS);
}
CAudioVideoRender::~CAudioVideoRender()
{
	mCriticalSectionPool->Enter(eCriticalSections::DecodedFrameCS);
	//----------------------------------------------------
	if (mRightImage != NULL)
	{
		delete mRightImage;
		mRightImage = NULL;
	}
	//----------------------------------------------------
	if (mLeftImage != NULL)
	{
		delete mLeftImage;
		mLeftImage = NULL;
	}
	//----------------------------------------------------
	if (mComPort != NULL)
	{
		delete mComPort;
		mComPort = NULL;
	}
	//----------------------------------------------------
	if (mAutoMemoryBase != NULL)
	{
		delete mAutoMemoryBase;
		mAutoMemoryBase = NULL;
	}
	//----------------------------------------------------
	mCriticalSectionPool->Leave(eCriticalSections::DecodedFrameCS);
	//----------------------------------------------------
	if (mCriticalSectionPool != NULL) 
	{ 
		delete mCriticalSectionPool;
		mCriticalSectionPool = NULL;
	}
}
CAudioVideoRender::eAudioVideoRenderErrors CAudioVideoRender::DrawWindow()
{
	eAudioVideoRenderErrors retVal = eAudioVideoRenderErrors::NoError;
	try
	{
		mCriticalSectionPool->Enter(eCriticalSections::DecodedFrameCS);
		//----------------------------------------------
		if (mAutoMemoryBase != NULL)
		{
			mAutoMemoryBase->Blt();
		}
		else
		{
			if (mRenderTarget == eVideoRenderTargets::GDI) retVal = eAudioVideoRenderErrors::DibIsNull;
			else if (mRenderTarget == eVideoRenderTargets::D2D) retVal = eAudioVideoRenderErrors::Direct2DIsNull;
			else if (mRenderTarget == eVideoRenderTargets::D3D) retVal = eAudioVideoRenderErrors::Direct3DIsNull;
		}
		//----------------------------------------------
		mCriticalSectionPool->Leave(eCriticalSections::DecodedFrameCS);
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAudioVideoRender::DrawAllWindows", "Exception in CAudioVideoRender DrawAllWindows");
	}
	return (retVal);
}
CAudioVideoRender::eAudioVideoRenderErrors CAudioVideoRender::VideoRender()
{
	try
	{
		if ((mLeftImage->Width != mRightImage->Width) || (mLeftImage->Height != mRightImage->Height)) return eAudioVideoRenderErrors::DifferentLeftRightImageDimensions;
		auto start = std::chrono::high_resolution_clock::now();
		//----------------------------------------------
		mCriticalSectionPool->Enter(eCriticalSections::DecodedFrameCS);
		//----------------------------------------------
		mImageToPlayIsLeft = !mImageToPlayIsLeft;
		if (mAutoMemoryBase != NULL)
		{
			if (mImageToPlayIsLeft)
			{
				mAutoMemoryBase->DrawImage(mHWnd, mLeftImage->PixelData.data(), mLeftImage->Width, mLeftImage->Height);
			}
			else
			{
				mAutoMemoryBase->DrawImage(mHWnd, mRightImage->PixelData.data(), mRightImage->Width, mRightImage->Height);
			}
		}
		else
		{
			if (mRenderTarget == eVideoRenderTargets::GDI) return eAudioVideoRenderErrors::DibIsNull;
			else if (mRenderTarget == eVideoRenderTargets::D2D) return eAudioVideoRenderErrors::Direct2DIsNull;
			else if (mRenderTarget == eVideoRenderTargets::D3D) return eAudioVideoRenderErrors::Direct3DIsNull;
		}
		eAudioVideoRenderErrors ret = DrawWindow();
		if (ret != eAudioVideoRenderErrors::NoError) return ret;
		if (mImageToPlayIsLeft)
		{
			if (mSignalSource == eSignalSources::COMPort) mComPort->SendCommand(mComPortName, CComPort::eTransparentLenses::Left);
		}
		else
		{
			if (mSignalSource == eSignalSources::COMPort) mComPort->SendCommand(mComPortName, CComPort::eTransparentLenses::Right);
		}
		//----------------------------------------------
		mCriticalSectionPool->Leave(eCriticalSections::DecodedFrameCS);
		//----------------------------------------------
		int tempDelay = 0;
		if (mFrequency == eFrequencies::Default) {
			int refreshRate = GetRefreshRate();
			tempDelay = (refreshRate > 0) ? (1000000 / refreshRate) : (1000000 / 60);
		}
		else if (mFrequency == eFrequencies::F1Hz) {
			tempDelay = 1000000 / 1;
		}
		else if (mFrequency == eFrequencies::F60Hz) {
			tempDelay = 1000000 / 60;
		}
		else if (mFrequency == eFrequencies::F75Hz) {
			tempDelay = 1000000 / 75;
		}
		else if (mFrequency == eFrequencies::F100Hz) {
			tempDelay = 1000000 / 100;
		}
		else if (mFrequency == eFrequencies::F120Hz) {
			tempDelay = 1000000 / 120;
		}
		else if (mFrequency == eFrequencies::F144Hz) {
			tempDelay = 1000000 / 144;
		}
		while (std::chrono::high_resolution_clock::now() - start < std::chrono::microseconds(tempDelay));
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CAudioVideoRender::VideoRender", "Exception in CAudioVideoRender VideoRender");
		return eAudioVideoRenderErrors::ExceptionInVideoRender;
	}
	return eAudioVideoRenderErrors::NoError;
}
int CAudioVideoRender::GetRefreshRate()
{
	// Structure to store display settings
	DEVMODE dm;
	// Initialize the memory block to zero
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);
	// Retrieve the current display settings
	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm)) 
	{
		return dm.dmDisplayFrequency;
	}
	return 0;
}