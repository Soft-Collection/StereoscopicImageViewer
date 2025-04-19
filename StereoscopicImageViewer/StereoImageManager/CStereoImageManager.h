#ifndef __CSTEREOIMAGEMANAGER_H__
#define __CSTEREOIMAGEMANAGER_H__

#include "CImage.h"
#include <thread>
#include <atomic>
#include "../StereoRendering/CStereoDirect3D.h"
#include "../SerialPort/CComPort.h"

class CStereoImageManager
{
private:
	HWND mHWnd;
	std::wstring mComPortName;
	CComPort* mComPort;
	CStereoDirect3D* mStereoDirect3D;
	//----------------------------------------
	CImage* mLeftImage;
	CImage* mRightImage;
	//----------------------------------------
	std::atomic<bool> m_ThreadRunning;
	std::thread m_Thread;
	//----------------------------------------
	bool mImageToPlayIsLeft;
private:
	void VideoRender();
	void ThreadFunction();
public:
	CStereoImageManager(HWND hWnd);
	~CStereoImageManager();
public:
	void DrawImage(LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath);
	void Start();
	void Stop();
	BOOL IsStarted();
	int GetFrequency();
	void SetCOMPort(LPCWSTR comPort);
	void SetGlassesTimeOffset(int offset);
	void SetTransparentTimePercent(int percent);
};
#endif // __CSTEREOIMAGEMANAGER_H__