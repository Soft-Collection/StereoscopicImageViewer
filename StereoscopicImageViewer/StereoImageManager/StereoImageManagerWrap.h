#include "CStereoImageManager.h"

extern "C" 
{
	__declspec(dllexport) void* StereoImageManagerCreateNew(HWND hWnd);
	__declspec(dllexport) void StereoImageManagerDispose(void* handle);
	__declspec(dllexport) void StereoImageManagerStereoStart(void* handle);
	__declspec(dllexport) void StereoImageManagerStereoStop(void* handle);
	__declspec(dllexport) BOOL StereoImageManagerStereoIsStarted(void* handle);
	__declspec(dllexport) int StereoImageManagerStereoGetFrequency(void* handle);
	__declspec(dllexport) void StereoImageManagerStereoSetCOMPort(void* handle, LPCWSTR comPort);
	__declspec(dllexport) void StereoImageManagerStereoSetGlassesTimeOffset(void* handle, int offset);
    __declspec(dllexport) void StereoImageManagerStereoLRBoth(void* handle, int lrboth);
    __declspec(dllexport) void StereoImageManagerStereoSwapLR(void* handle, BOOL swaplr);
    __declspec(dllexport) void StereoImageManagerStereoWindowSizeChanged(void* handle);
	__declspec(dllexport) void StereoImageManagerImagerProvideImages(void* handle, LPCWSTR leftImageFilePath, LPCWSTR rightImageFilePath);
}