#include "stdafx.h"
#include "StereoImageManagerWrap.h"

void* StereoImageManagerCreateNew(HWND hWnd)
{
	CStereoImageManager* cStereoImageManager = new CStereoImageManager(hWnd);
	return((void*)cStereoImageManager);
}
void StereoImageManagerDispose(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	delete cStereoImageManager;
}
void StereoImageManagerDrawImage(void* handle, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->DrawImage(leftImageFilePath, rightImageFilePath);
}
void StereoImageManagerStart(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->Start();
}
void StereoImageManagerStop(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->Stop();
}
BOOL StereoImageManagerIsStarted(void* handle)
{
	if (handle == NULL) return false;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->IsStarted());
}
int StereoImageManagerGetFrequency(void* handle)
{
	if (handle == NULL) return 0;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->GetFrequency());
}
void StereoImageManagerSetCOMPort(void* handle, LPCWSTR comPort)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->SetCOMPort(comPort);
}
void StereoImageManagerSetGlassesTimeOffset(void* handle, int offset)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->SetGlassesTimeOffset(offset);
}
void StereoImageManagerSetTransparentTimePercent(void* handle, int percent)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->SetTransparentTimePercent(percent);
}
