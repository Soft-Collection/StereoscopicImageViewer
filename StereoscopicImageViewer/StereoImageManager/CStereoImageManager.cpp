#include "stdafx.h"
#include "CStereoImageManager.h"
#include "../Common/CExceptionReport.h"
#include "../Common/CCriticalSectionPool.h"
#include <thread>
#include <Windows.h>
#include <iostream>

CStereoImageManager::CStereoImageManager(HWND hWnd, LPCWSTR comPort, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath)
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::Size);
	mCriticalSectionPool->Enter(eCriticalSections::DecodedFrameCS);
	//----------------------------------------------------
	mHWnd = hWnd;
	mStereoDirect3D = new CStereoDirect3D();
	mComPortName = std::wstring(comPort);
	mComPort = NULL;
	mComPort = new CComPort();
	//----------------------------------------------------
	mLeftImage = new CImage();
	mLeftImage->FilePath = std::wstring(leftImageFilePath);
	mLeftImage->IsLeft = true;
	mLeftImage->RectangleHeight = 30;
	CImage::LoadPNG(mLeftImage);
	mRightImage = new CImage();
	mRightImage->FilePath = std::wstring(rightImageFilePath);
	mRightImage->IsLeft = false;
	mRightImage->RectangleHeight = 30;
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
			if (mComPort != NULL)
			{
				mComPort->SendSync(mComPortName);
			}
		}
		//----------------------------------------------
		mCriticalSectionPool->Leave(eCriticalSections::DecodedFrameCS);
		//----------------------------------------------
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CStereoImageManager::VideoRender", "Exception in CStereoImageManager VideoRender");
		return eStereoImageManagerErrors::ExceptionInVideoRender;
	}
	return eStereoImageManagerErrors::NoError;
}
CStereoImageManager::eStereoImageManagerErrors CStereoImageManager::SetGlassesTimeOffset(int offset)
{
	try
	{
		//----------------------------------------------
		mCriticalSectionPool->Enter(eCriticalSections::DecodedFrameCS);
		//----------------------------------------------
		if (mComPort != NULL)
		{
			mComPort->SendGlassesTimeOffset(mComPortName, offset);
		}
		//----------------------------------------------
		mCriticalSectionPool->Leave(eCriticalSections::DecodedFrameCS);
		//----------------------------------------------
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CStereoImageManager::VideoRender", "Exception in CStereoImageManager SetGlassesTimeOffset");
		return eStereoImageManagerErrors::ExceptionInVideoRender;
	}
	return eStereoImageManagerErrors::NoError;
}
CStereoImageManager::eStereoImageManagerErrors CStereoImageManager::SetTransparentTimePercent(int percent)
{
	try
	{
		//----------------------------------------------
		mCriticalSectionPool->Enter(eCriticalSections::DecodedFrameCS);
		//----------------------------------------------
		if (mComPort != NULL)
		{
			mComPort->SendTransparentTimePercent(mComPortName, percent);
		}
		//----------------------------------------------
		mCriticalSectionPool->Leave(eCriticalSections::DecodedFrameCS);
		//----------------------------------------------
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CStereoImageManager::VideoRender", "Exception in CStereoImageManager SetTransparentTimePercent");
		return eStereoImageManagerErrors::ExceptionInVideoRender;
	}
	return eStereoImageManagerErrors::NoError;
}
