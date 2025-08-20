#include "stdafx.h"
#include "CStereoImageManager.h"
#include <thread>
#include <Windows.h>
#include <iostream>

CStereoImageManager::CStereoImageManager(HWND hWnd)
{
	mHWnd = hWnd;
	mStereoDirect2D = new CStereoDirect2D(hWnd);
	mComPortName = std::wstring(L"");
	mComPort = NULL;
	//----------------------------------------------------
	mLeftImage = NULL;
	mRightImage = NULL;
	//----------------------------------------------------
	mMutexRender1 = new std::mutex();
	mMutexRender2 = new std::mutex();
	mThreadRenderRunning.store(false);
	mThreadRender = nullptr;
	mMutexCOMPort = new std::mutex();
	//----------------------------------------------------
	mImageToPlayIsLeft = true;
	//----------------------------------------------------
}
CStereoImageManager::~CStereoImageManager()
{
	StereoStop();
	//----------------------------------------------------
	std::unique_lock<std::mutex> lock1(*mMutexCOMPort); // Lock the mutex
	if (mComPort != NULL)
	{
		delete mComPort;
		mComPort = NULL;
	}
	lock1.unlock();
	//----------------------------------------------------
	std::unique_lock<std::mutex> lock2(*mMutexRender1); // Lock the mutex
	std::unique_lock<std::mutex> lock5(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		delete mStereoDirect2D;
		mStereoDirect2D = NULL;
	}
	if (mRightImage != NULL)
	{
		delete mRightImage;
		mRightImage = NULL;
	}
	if (mLeftImage != NULL)
	{
		delete mLeftImage;
		mLeftImage = NULL;
	}
	lock5.unlock();
	lock2.unlock();
	//----------------------------------------------------
	if (mMutexCOMPort != nullptr)
	{
		delete mMutexCOMPort;
		mMutexCOMPort = nullptr;
	}
	//----------------------------------------------------
	if (mMutexRender2 != nullptr)
	{
		delete mMutexRender2;
		mMutexRender2 = nullptr;
	}
	//----------------------------------------------------
	if (mMutexRender1 != nullptr)
	{
		delete mMutexRender1;
		mMutexRender1 = nullptr;
	}
}
void CStereoImageManager::StereoStart()
{
	if (!mThreadRenderRunning.load())
	{
		mThreadRenderRunning = true;
		mThreadRender = new std::thread(&CStereoImageManager::ThreadRenderFunction, this);
	}
}
void CStereoImageManager::StereoStop()
{
	if (mThreadRenderRunning.load())
	{
		mThreadRenderRunning.store(false);
		if (mThreadRender && mThreadRender->joinable())
		{
			mThreadRender->join();
		}
		delete mThreadRender;
		mThreadRender = nullptr;
	}
	std::unique_lock<std::mutex> lock1(*mMutexRender1); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->Blt(TRUE, this, nullptr);
	}
	lock1.unlock();
}
BOOL CStereoImageManager::StereoIsStarted()
{
	return mThreadRenderRunning.load();
}
int CStereoImageManager::StereoGetFrequency()
{
	std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		return mStereoDirect2D->GetFrequency();
	}
	lock1.unlock();
	return 0;
}
void CStereoImageManager::StereoSetCOMPort(LPCWSTR comPort)
{
	if (mComPortName != std::wstring(comPort))
	{
		std::unique_lock<std::mutex> lock1(*mMutexCOMPort); // Lock the mutex
		if (mComPort != NULL)
		{
			delete mComPort;
			mComPort = NULL;
		}
		mComPort = new CComPort(std::wstring(comPort));
		mComPortName = std::wstring(comPort);
		lock1.unlock();
	}
}
void CStereoImageManager::StereoSetGlassesTimeOffset(int offset)
{
	std::unique_lock<std::mutex> lock1(*mMutexCOMPort); // Lock the mutex
	if (mComPort != NULL)
	{
		mComPort->SendGlassesTimeOffset(offset);
	}
	lock1.unlock();
}
void CStereoImageManager::StereoLRBoth(int lrboth)
{
	std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->StereoLRBoth(lrboth);
	}
	lock1.unlock();
}
void CStereoImageManager::StereoSwapLR(BOOL swaplr)
{
	std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->StereoSwapLR(swaplr);
	}
	lock1.unlock();
}
void CStereoImageManager::StereoWindowSizeChanged()
{
	std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->StereoWindowSizeChanged();
	}
	lock1.unlock();
}
void CStereoImageManager::ImagerProvideImages(LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath)
{
	mLeftImage = CImage::LoadImage(std::wstring(leftImageFilePath), true);
	mRightImage = CImage::LoadImage(std::wstring(rightImageFilePath), false);
	//----------------------------------------------
	std::unique_lock<std::mutex> lock2(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->DrawImage(mLeftImage, mRightImage);
	}
	lock2.unlock();
	//----------------------------------------------
	mImageToPlayIsLeft = true;
}
void CStereoImageManager::ThreadRenderFunction()
{
	while (mThreadRenderRunning.load())
	{
		std::unique_lock<std::mutex> lock1(*mMutexRender1); // Lock the mutex
		mImageToPlayIsLeft = !mImageToPlayIsLeft;
		if (mStereoDirect2D != NULL)
		{
			mStereoDirect2D->Blt(mImageToPlayIsLeft, this, SendSyncStatic);
		}
		lock1.unlock();
	}
}
void CStereoImageManager::SendSyncStatic(void* user, bool isLeft)
{
	if (user == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)user;
	cStereoImageManager->SendSync(isLeft);
}
void CStereoImageManager::SendSync(bool isLeft)
{
	std::unique_lock<std::mutex> lock1(*mMutexCOMPort); // Lock the mutex
	if (mComPort != NULL)
	{
		mComPort->SendSync(isLeft);
	}
	lock1.unlock();
}