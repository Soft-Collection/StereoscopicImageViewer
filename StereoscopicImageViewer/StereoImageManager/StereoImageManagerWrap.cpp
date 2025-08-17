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
void StereoImageManagerStereoStart(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoStart();
}
void StereoImageManagerStereoStop(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoStop();
}
BOOL StereoImageManagerStereoIsStarted(void* handle)
{
	if (handle == NULL) return false;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->StereoIsStarted());
}
int StereoImageManagerStereoGetFrequency(void* handle)
{
	if (handle == NULL) return 0;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->StereoGetFrequency());
}
void StereoImageManagerStereoSetCOMPort(void* handle, LPCWSTR comPort)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoSetCOMPort(comPort);
}
void StereoImageManagerStereoSetGlassesTimeOffset(void* handle, int offset)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoSetGlassesTimeOffset(offset);
}
void StereoImageManagerStereoLRBoth(void* handle, int lrboth)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoLRBoth(lrboth);
}
void StereoImageManagerStereoSwapLR(void* handle, BOOL swaplr)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoSwapLR(swaplr);
}
void StereoImageManagerStereoWindowSizeChanged(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->StereoWindowSizeChanged();
}
void StereoImageManagerImagerProvideImages(void* handle, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	cStereoImageManager->ImagerProvideImages(leftImageFilePath, rightImageFilePath);
}