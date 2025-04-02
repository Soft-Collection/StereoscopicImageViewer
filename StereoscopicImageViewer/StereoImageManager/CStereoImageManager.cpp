#include "stdafx.h"
#include "CStereoImageManager.h"
#include "../Common/CExceptionReport.h"
#include "../Common/CCriticalSectionPool.h"
#include <chrono>
#include <thread>
#include <Windows.h>
#include <iostream>

CStereoImageManager::CStereoImageManager(HWND hWnd, eFrequencies frequency, eSignalSources signalSource, LPCWSTR comPort, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath)
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::Size);
	mCriticalSectionPool->Enter(eCriticalSections::DecodedFrameCS);
	//----------------------------------------------------
	mHWnd = hWnd;
	mStereoDirect3D = new CStereoDirect3D();
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
	mStereoDirect3D->DrawImage(mHWnd, mLeftImage->PixelData.data(), mRightImage->PixelData.data(), mLeftImage->Width, mLeftImage->Height);
	//----------------------------------------------------
	mImageToPlayIsLeft = true;
	//----------------------------------------------------
	mCriticalSectionPool->Leave(eCriticalSections::DecodedFrameCS);
}
CStereoImageManager::~CStereoImageManager()
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
	if (mStereoDirect3D != NULL)
	{
		delete mStereoDirect3D;
		mStereoDirect3D = NULL;
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
CStereoImageManager::eStereoImageManagerErrors CStereoImageManager::VideoRender()
{
	try
	{
		if ((mLeftImage->Width != mRightImage->Width) || (mLeftImage->Height != mRightImage->Height)) return eStereoImageManagerErrors::DifferentLeftRightImageDimensions;
		auto start = std::chrono::high_resolution_clock::now();
		//----------------------------------------------
		mCriticalSectionPool->Enter(eCriticalSections::DecodedFrameCS);
		//----------------------------------------------
		mImageToPlayIsLeft = !mImageToPlayIsLeft;
		if (mStereoDirect3D != NULL)
		{
			mStereoDirect3D->Blt(mImageToPlayIsLeft);
		}
		else
		{
			return eStereoImageManagerErrors::Direct3DIsNull;
		}
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
		CExceptionReport::WriteExceptionReportToFile("CStereoImageManager::VideoRender", "Exception in CStereoImageManager VideoRender");
		return eStereoImageManagerErrors::ExceptionInVideoRender;
	}
	return eStereoImageManagerErrors::NoError;
}
int CStereoImageManager::GetRefreshRate()
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