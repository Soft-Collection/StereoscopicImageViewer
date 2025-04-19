#include "stdafx.h"
#include "CStereoImageManager.h"
#include <thread>
#include <Windows.h>
#include <iostream>

CStereoImageManager::CStereoImageManager(HWND hWnd)
{
	mHWnd = hWnd;
	mStereoDirect3D = new CStereoDirect3D(hWnd);
	mComPortName = std::wstring(L"");
	mComPort = NULL;
	//----------------------------------------------------
	mLeftImage = NULL;
	mRightImage = NULL;
	//----------------------------------------------------
	m_ThreadRunning.store(false);
	m_Thread;
	//----------------------------------------------------
	mImageToPlayIsLeft = true;
	//----------------------------------------------------
}
CStereoImageManager::~CStereoImageManager()
{
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
}
void CStereoImageManager::DrawImage(LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath)
{
	mLeftImage = CImage::LoadImage(std::wstring(leftImageFilePath), true);
	mRightImage = CImage::LoadImage(std::wstring(rightImageFilePath), false);
	//----------------------------------------------
	CStereoDirect3D::ImageData left = { mLeftImage->PixelData.data(), mLeftImage->Width, mLeftImage->Height, mLeftImage->Channels, nullptr, nullptr };
	CStereoDirect3D::ImageData right = { mRightImage->PixelData.data(), mRightImage->Width, mRightImage->Height, mRightImage->Channels, nullptr, nullptr };
	mStereoDirect3D->DrawImage(left, right);
	//----------------------------------------------
	mImageToPlayIsLeft = true;
}
void CStereoImageManager::VideoRender()
{
	mImageToPlayIsLeft = !mImageToPlayIsLeft;
	if (mStereoDirect3D != NULL)
	{
		mStereoDirect3D->Blt(mImageToPlayIsLeft);
	}
	if (mImageToPlayIsLeft)
	{
		if (mComPort != NULL)
		{
			mComPort->SendSync();
		}
	}
}
void CStereoImageManager::Start()
{
	if (!m_ThreadRunning.load()) 
	{
		m_ThreadRunning = true;
		m_Thread = std::thread(&CStereoImageManager::ThreadFunction, this);
	}
}
void CStereoImageManager::Stop()
{
	if (m_ThreadRunning.load())
	{
		m_ThreadRunning = false;
		if (m_Thread.joinable()) 
		{
			m_Thread.join();  // Wait for thread to finish
		}
	}
}
BOOL CStereoImageManager::IsStarted()
{
	return m_ThreadRunning.load();
}
int CStereoImageManager::GetFrequency()
{
	if (mStereoDirect3D != NULL)
	{
		return mStereoDirect3D->GetFrequency();
	}
}
void CStereoImageManager::SetCOMPort(LPCWSTR comPort)
{
	if (mComPortName != std::wstring(comPort))
	{
		if (mComPort != NULL)
		{
			delete mComPort;
			mComPort = NULL;
		}
		mComPort = new CComPort(std::wstring(comPort));
		mComPortName = std::wstring(comPort);
	}
}
void CStereoImageManager::SetGlassesTimeOffset(int offset)
{
	if (mComPort != NULL)
	{
		mComPort->SendGlassesTimeOffset(offset);
	}
}
void CStereoImageManager::SetTransparentTimePercent(int percent)
{
	if (mComPort != NULL)
	{
		mComPort->SendTransparentTimePercent(percent);
	}
}
void CStereoImageManager::ThreadFunction() {
	while (m_ThreadRunning.load()) 
	{
		VideoRender();
	}
}
