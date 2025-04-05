#include "CStereoImageManager.h"

extern "C" 
{
	__declspec(dllexport) void* StereoImageManagerCreateNew(HWND hWnd, CStereoImageManager::eFrequencies frequency, CStereoImageManager::eSignalSources signalSource, LPCWSTR comPort, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath);
	__declspec(dllexport) void StereoImageManagerDispose(void* handle);
	__declspec(dllexport) CStereoImageManager::eStereoImageManagerErrors StereoImageManagerVideoRender(void* handle);
	__declspec(dllexport) CStereoImageManager::eStereoImageManagerErrors StereoImageManagerSetGlassesTimeOffset(void* handle, int offset);
	__declspec(dllexport) CStereoImageManager::eStereoImageManagerErrors StereoImageManagerSetTransparentTimePercent(void* handle, int percent);
}