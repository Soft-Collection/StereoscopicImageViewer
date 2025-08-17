#ifndef __CSTEREOIMAGEMANAGER_H__
#define __CSTEREOIMAGEMANAGER_H__

#include <mutex>
#include <thread>
#include <atomic>
#include "../StereoRendering/CStereoDirect2D.h"
#include "../SerialPort/CComPort.h"
#include "../Imager/CImage.h"

class CStereoImageManager
{
private:
	HWND mHWnd;
	std::wstring mComPortName;
	CComPort* mComPort;
	CStereoDirect2D* mStereoDirect2D;
	//----------------------------------------
	CImage* mLeftImage;
	CImage* mRightImage;
	//----------------------------------------
	std::mutex* mMutexRender1;
	std::mutex* mMutexRender2;
	std::atomic<bool> mThreadRenderRunning;
	std::thread* mThreadRender;
	std::mutex* mMutexCOMPort;
	//----------------------------------------
	bool mImageToPlayIsLeft;
	//----------------------------------------
private:
	void ThreadRenderFunction();
	static void SendSyncStatic(void* user, int syncType);
	void SendSync(int syncType);
public:
	CStereoImageManager(HWND hWnd);
	~CStereoImageManager();
public:
	void StereoStart();
	void StereoStop();
	BOOL StereoIsStarted();
	int StereoGetFrequency();
	void StereoSetCOMPort(LPCWSTR comPort);
	void StereoSetGlassesTimeOffset(int offset);
	void StereoLRBoth(int lrboth);
	void StereoSwapLR(BOOL swaplr);
	void StereoWindowSizeChanged();
	void ImagerProvideImages(LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath);
};
#endif // __CSTEREOIMAGEMANAGER_H__