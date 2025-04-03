#include "stdafx.h"
#include "StereoImageManagerWrap.h"

void* StereoImageManagerCreateNew(HWND hWnd, CStereoImageManager::eFrequencies frequency, CStereoImageManager::eSignalSources signalSource, LPCWSTR comPort, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath)
{
	CStereoImageManager* cStereoImageManager = new CStereoImageManager(hWnd, frequency, signalSource, comPort, leftImageFilePath, rightImageFilePath);
	return((void*)cStereoImageManager);
}
void StereoImageManagerDispose(void* handle)
{
	if (handle == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	delete cStereoImageManager;
}
CStereoImageManager::eStereoImageManagerErrors StereoImageManagerVideoRender(void* handle)
{
	if (handle == NULL) return(CStereoImageManager::eStereoImageManagerErrors::NullHandle);
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)handle;
	return(cStereoImageManager->VideoRender());
}
